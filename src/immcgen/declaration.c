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
    } else if (symbol_dtype->type == DTYPE_FUNCUCTION) {
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
    Vector* codes = new_vector(&t_immc);
    Dtype* dtype = immcgen->initialized_dtype;

    for (int i = 0; i < dtype->array->size; i++) {
        immcgen->initialized_dtype = dtype->array->of_dtype;
        append_child_immcode(immcgen, codes, i);
    }

    return codes;
}

Vector* gen_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = new_mem_immcope(immcgen->initialized_offset);

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    vector_push(codes, new_inst_immc(IMMC_INST_STORE, dst, src, NULL));
    immcgen->initialized_offset -= dtype_size(immcgen->initialized_dtype);

    return codes;
}
