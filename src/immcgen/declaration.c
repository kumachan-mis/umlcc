#include "./declaration.h"
#include "../common/type.h"
#include "../dtype/dtype.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_decl_list_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_children_immcode(immcgen, codes);
    return codes;
}

Vector* gen_init_decl_immcode(Immcgen* immcgen) {
    Vector* gen_global_init_decl_immcode(Immcgen * immcgen);
    Vector* gen_local_init_decl_immcode(Immcgen * immcgen);

    if (immcgen->local_table == NULL) return gen_global_init_decl_immcode(immcgen);
    return gen_local_init_decl_immcode(immcgen);
}

Vector* gen_global_init_decl_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    append_child_immcode(immcgen, codes, 0);

    Srt* decl_srt = vector_at(immcgen->srt->children, 0);
    Symbol* decl_symbol = symboltable_search(immcgen->global_table, decl_srt->ident_name);
    if (!dtype_isobject(decl_symbol->dtype)) return codes;

    char* label_name = new_string(decl_srt->ident_name);
    vector_push(codes, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, label_name));

    if (vector_size(immcgen->srt->children) == 1) {
        IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, dtype_size(decl_srt->dtype));
        vector_push(codes, new_int_data_immc(IMMC_DATA_ZERO, iliteral));
        return codes;
    }

    immcgen->initialized_dtype = decl_symbol->dtype;
    append_child_immcode(immcgen, codes, 1);
    immcgen->initialized_dtype = NULL;

    return codes;
}

Vector* gen_local_init_decl_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    append_child_immcode(immcgen, codes, 0);

    if (vector_size(immcgen->srt->children) == 1) return codes;

    Srt* decl_srt = vector_at(immcgen->srt->children, 0);
    Symbol* decl_symbol = symboltable_search(immcgen->local_table, decl_srt->ident_name);

    immcgen->initialized_dtype = decl_symbol->dtype;
    immcgen->initialized_offset = decl_symbol->memory_offset;
    append_child_immcode(immcgen, codes, 1);
    immcgen->initialized_offset = -1;
    immcgen->initialized_dtype = NULL;

    return codes;
}

Vector* gen_decl_immcode(Immcgen* immcgen) {
    Srt* srt = immcgen->srt;

    char* symbol_name = new_string(srt->ident_name);
    Dtype* symbol_dtype = dtype_copy(srt->dtype);

    if (immcgen->local_table == NULL) {
        SymbolTable* table = immcgen->global_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else if (!dtype_isobject(symbol_dtype)) {
        SymbolTable* table = immcgen->local_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else {
        SymbolTable* table = immcgen->local_table;
        symboltable_define_memory(table, symbol_name, symbol_dtype);
    }

    return new_vector(&t_immc);
}

Vector* gen_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_array_initializer_immcode(Immcgen * immcgen);
    Vector* gen_scalar_initializer_immcode(Immcgen * immcgen);

    Dtype* dtype = immcgen->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_ARRAY:
            return gen_array_initializer_immcode(immcgen);
        default:
            return gen_scalar_initializer_immcode(immcgen);
    }
}

Vector* gen_array_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_global_string_initializer_immcode();
    Vector* gen_local_string_initializer_immcode();

    Srt* srt = vector_at(immcgen->srt->children, 0);
    if (srt->type == SRT_STRING_EXPR) {
        if (immcgen->local_table == NULL) return gen_global_string_initializer_immcode(immcgen);
        return gen_local_string_initializer_immcode(immcgen);
    }

    Vector* codes = new_vector(&t_immc);
    Dtype* dtype = immcgen->initialized_dtype;

    for (int i = 0; i < dtype->array->size; i++) {
        immcgen->initialized_dtype = dtype->array->of_dtype;
        append_child_immcode(immcgen, codes, i);
    }

    return codes;
}

Vector* gen_global_string_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = vector_at(immcgen->srt->children, 0);

    StringLiteral* sliteral = sliteral_copy(srt->sliteral);
    vector_push(codes, new_str_data_immc(IMMC_DATA_STR, sliteral));

    return codes;
}

Vector* gen_local_string_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = vector_at(immcgen->srt->children, 0);

    ImmcOpe* dst = new_mem_immcope(immcgen->initialized_offset);
    ImmcOpe* src = new_str_immcope(sliteral_copy(srt->sliteral));

    vector_push(codes, new_inst_immc(IMMC_INST_STR, dst, src, NULL));
    immcgen->initialized_offset -= dtype_size(immcgen->initialized_dtype);

    return codes;
}

Vector* gen_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_global_scalar_initializer_immcode(Immcgen * immcgen);
    Vector* gen_local_scalar_initializer_immcode(Immcgen * immcgen);

    if (immcgen->local_table == NULL) return gen_global_scalar_initializer_immcode(immcgen);
    return gen_local_scalar_initializer_immcode(immcgen);
}

Vector* gen_global_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* srt = vector_at(immcgen->srt->children, 0);
    // TODO: support expression for global initializer
    while (srt->type == SRT_CAST_EXPR) {
        srt = vector_at(srt->children, 0);
    }

    int initialized_size = dtype_size(immcgen->initialized_dtype);
    IntegerLiteral* iliteral = NULL;
    switch (initialized_size) {
        case 1:
            iliteral = iliteral_copy(srt->iliteral);
            vector_push(codes, new_int_data_immc(IMMC_DATA_BYTE, iliteral));
            break;
        case 2:
            iliteral = iliteral_copy(srt->iliteral);
            vector_push(codes, new_int_data_immc(IMMC_DATA_WORD, iliteral));
            break;
        case 4:
            iliteral = iliteral_copy(srt->iliteral);
            vector_push(codes, new_int_data_immc(IMMC_DATA_LONG, iliteral));
            break;
        case 8:
            iliteral = iliteral_copy(srt->iliteral);
            vector_push(codes, new_int_data_immc(IMMC_DATA_QUAD, iliteral));
            break;
        default:
            fprintf(stderr, "Error: unexpected global variable size %d\n", initialized_size);
            exit(1);
    }

    return codes;
}

Vector* gen_local_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = new_mem_immcope(immcgen->initialized_offset);
    ImmcOpe* src = gen_child_int_immcope(immcgen, codes, 0);

    vector_push(codes, new_inst_immc(IMMC_INST_STORE, dst, src, NULL));
    immcgen->initialized_offset -= dtype_size(immcgen->initialized_dtype);

    return codes;
}
