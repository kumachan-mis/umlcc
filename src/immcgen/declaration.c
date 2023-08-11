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

Vector* gen_tag_decl_immcode(Immcgen* immcgen) {
    Srt* srt = immcgen->srt;

    if (srt->dtype->type == DTYPE_STRUCT) {
        char* struct_name = new_string(srt->ident_name);
        Vector* struct_members = vector_copy(srt->dtype->dstruct->members);
        tagtable_define_struct(immcgen->tag_table, struct_name, struct_members);
    }

    return new_vector(&t_immc);
}

Vector* gen_init_decl_immcode(Immcgen* immcgen) {
    Vector* gen_global_init_decl_immcode(Immcgen * immcgen);
    Vector* gen_local_init_decl_immcode(Immcgen * immcgen);

    if (immcgen->symbol_table->outer_scope == NULL) {
        return gen_global_init_decl_immcode(immcgen);
    } else {
        return gen_local_init_decl_immcode(immcgen);
    }
}

Vector* gen_global_init_decl_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    append_child_immcode(immcgen, codes, 0);

    Srt* decl_srt = vector_at(immcgen->srt->children, 0);
    Symbol* decl_symbol = symboltable_search(immcgen->symbol_table, decl_srt->ident_name);
    if (!dtype_isobject(decl_symbol->dtype)) {
        return codes;
    }

    char* label_name = new_string(decl_srt->ident_name);
    vector_push(codes, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, label_name));

    if (vector_size(immcgen->srt->children) == 1) {
        IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, dtype_nbytes(decl_srt->dtype));
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

    if (vector_size(immcgen->srt->children) == 1) {
        return codes;
    }

    Srt* decl_srt = vector_at(immcgen->srt->children, 0);
    Symbol* decl_symbol = symboltable_search(immcgen->symbol_table, decl_srt->ident_name);

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
    DType* symbol_dtype = dtype_copy(srt->dtype);

    if (immcgen->symbol_table->outer_scope == NULL) {
        symboltable_define_label(immcgen->symbol_table, symbol_name, symbol_dtype);
    } else if (!dtype_isobject(symbol_dtype)) {
        symboltable_define_label(immcgen->symbol_table, symbol_name, symbol_dtype);
    } else {
        symboltable_define_memory(immcgen->symbol_table, symbol_name, symbol_dtype);
    }

    return new_vector(&t_immc);
}

Vector* gen_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_array_initializer_immcode(Immcgen * immcgen);
    Vector* gen_struct_initializer_immcode(Immcgen * immcgen);
    Vector* gen_scalar_initializer_immcode(Immcgen * immcgen);

    DType* dtype = immcgen->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_ARRAY:
            return gen_array_initializer_immcode(immcgen);
        case DTYPE_STRUCT:
            if (dtype->dstruct->members == NULL) {
                immcgen->initialized_dtype = tagtable_search(immcgen->tag_table, dtype->dstruct->name);
            }
            return gen_struct_initializer_immcode(immcgen);
        default:
            return gen_scalar_initializer_immcode(immcgen);
    }
}

Vector* gen_array_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_global_string_initializer_immcode(Immcgen * immcgen);
    Vector* gen_local_string_initializer_immcode(Immcgen * immcgen);

    Srt* srt = vector_at(immcgen->srt->children, 0);
    if (srt->type == SRT_STRING_EXPR) {
        if (immcgen->symbol_table->outer_scope == NULL) {
            return gen_global_string_initializer_immcode(immcgen);
        } else {
            return gen_local_string_initializer_immcode(immcgen);
        }
    }

    Vector* codes = new_vector(&t_immc);
    DType* dtype = immcgen->initialized_dtype;

    for (int i = 0; i < dtype->darray->size; i++) {
        immcgen->initialized_dtype = dtype->darray->of_dtype;
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
    immcgen->initialized_offset -= dtype_nbytes(immcgen->initialized_dtype);

    return codes;
}

Vector* gen_struct_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_global_struct_initializer_immcode(Immcgen * immcgen);
    Vector* gen_local_struct_initializer_immcode(Immcgen * immcgen);

    if (immcgen->symbol_table->outer_scope == NULL) {
        return gen_global_struct_initializer_immcode(immcgen);
    } else {
        return gen_local_struct_initializer_immcode(immcgen);
    }
}

Vector* gen_global_struct_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    DType* dtype = immcgen->initialized_dtype;

    int num_members = vector_size(dtype->dstruct->members);
    for (int i = 0; i < num_members; i++) {
        DStructMember* member = vector_at(dtype->dstruct->members, i);
        immcgen->initialized_dtype = member->dtype;

        append_child_immcode(immcgen, codes, i);

        int next_offset = 0;
        if (i == num_members - 1) {
            next_offset = dtype_nbytes(dtype);
        } else {
            DStructMember* next_dstructmember = vector_at(dtype->dstruct->members, i + 1);
            next_offset = next_dstructmember->memory_offset;
        }

        int padding = next_offset - (member->memory_offset + dtype_nbytes(member->dtype));
        if (padding > 0) {
            vector_push(codes, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, padding)));
        }
    }

    return codes;
}

Vector* gen_local_struct_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    DType* dtype = immcgen->initialized_dtype;
    int offset = immcgen->initialized_offset;

    int num_members = vector_size(dtype->dstruct->members);
    for (int i = 0; i < num_members; i++) {
        DStructMember* member = vector_at(dtype->dstruct->members, i);
        immcgen->initialized_dtype = member->dtype;
        immcgen->initialized_offset = offset - member->memory_offset;
        append_child_immcode(immcgen, codes, i);
    }

    return codes;
}

Vector* gen_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* gen_global_scalar_initializer_immcode(Immcgen * immcgen);
    Vector* gen_local_scalar_initializer_immcode(Immcgen * immcgen);

    if (immcgen->symbol_table->outer_scope == NULL) {
        return gen_global_scalar_initializer_immcode(immcgen);
    } else {
        return gen_local_scalar_initializer_immcode(immcgen);
    }
}

Vector* gen_global_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* srt = vector_at(immcgen->srt->children, 0);
    // TODO: support expression for global initializer
    while (srt->type == SRT_CAST_EXPR) {
        srt = vector_at(srt->children, 0);
    }

    int initialized_size = dtype_nbytes(immcgen->initialized_dtype);
    switch (initialized_size) {
        case 1:
            vector_push(codes, new_int_data_immc(IMMC_DATA_BYTE, iliteral_copy(srt->iliteral)));
            break;
        case 2:
            vector_push(codes, new_int_data_immc(IMMC_DATA_WORD, iliteral_copy(srt->iliteral)));
            break;
        case 4:
            vector_push(codes, new_int_data_immc(IMMC_DATA_LONG, iliteral_copy(srt->iliteral)));
            break;
        case 8:
            vector_push(codes, new_int_data_immc(IMMC_DATA_QUAD, iliteral_copy(srt->iliteral)));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_global_scalar_initializer_immcode)\n");
            exit(1);
    }

    return codes;
}

Vector* gen_local_scalar_initializer_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = new_mem_immcope(immcgen->initialized_offset);
    ImmcOpe* src = gen_child_int_immcope(immcgen, codes, 0);

    vector_push(codes, new_inst_immc(IMMC_INST_STORE, dst, src, NULL));
    immcgen->initialized_offset -= dtype_nbytes(immcgen->initialized_dtype);

    return codes;
}
