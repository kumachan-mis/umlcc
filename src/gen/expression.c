#include "./expression.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_assignment_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* dest = new_ptr_immcope(codegen->virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* src = new_reg_immcope(codegen->virtual_reg_id);

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            vector_push(codes, new_inst_immc(INST_STORE, dest, src, NULL));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_logical_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(codegen->virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(codegen->virtual_reg_id);

    codegen->virtual_reg_id++;
    ImmcOpe* dest = new_reg_immcope(codegen->virtual_reg_id);

    switch (srt->type) {
        case SRT_LOR_EXPR:
            vector_push(codes, new_inst_immc(INST_LOR, dest, fst_src, snd_src));
            break;
        case SRT_LAND_EXPR:
            vector_push(codes, new_inst_immc(INST_LAND, dest, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_additive_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(codegen->virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(codegen->virtual_reg_id);

    codegen->virtual_reg_id++;
    ImmcOpe* dest = new_reg_immcope(codegen->virtual_reg_id);

    switch (srt->type) {
        case SRT_ADD_EXPR:
            vector_push(codes, new_inst_immc(INST_ADD, dest, fst_src, snd_src));
            break;
        case SRT_SUB_EXPR:
            vector_push(codes, new_inst_immc(INST_SUB, dest, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_multiplicative_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(codegen->virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(codegen->virtual_reg_id);

    codegen->virtual_reg_id++;
    ImmcOpe* dest = new_reg_immcope(codegen->virtual_reg_id);

    switch (srt->type) {
        case SRT_MUL_EXPR:
            vector_push(codes, new_inst_immc(INST_MUL, dest, fst_src, snd_src));
            break;
        case SRT_DIV_EXPR:
            vector_push(codes, new_inst_immc(INST_DIV, dest, fst_src, snd_src));
            break;
        case SRT_MOD_EXPR:
            vector_push(codes, new_inst_immc(INST_MOD, dest, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_postfix_expr_code(Codegen* codegen) {
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;
    Vector* gen_call_expr_code(Codegen * codegen);

    switch (srt->type) {
        case SRT_CALL_EXPR:
            codes = gen_call_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_call_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    Srt* param_srt = vector_at(srt->children, 1);
    int num_args = vector_size(param_srt->children);
    vector_push(codes, new_inst_immc(INST_PREP, NULL, new_imm_immcope(num_args), NULL));

    codegen->_srt = param_srt;
    for (int i = num_args - 1; i >= 0; i--) {
        append_child_code(codegen, codes, i);
        ImmcOpe* fst_src = new_imm_immcope(i);
        ImmcOpe* snd_src = new_reg_immcope(codegen->virtual_reg_id);
        vector_push(codes, new_inst_immc(INST_STARG, NULL, fst_src, snd_src));
    }
    codegen->_srt = srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_ptr_immcope(codegen->virtual_reg_id);
    ImmcOpe* snd_src = new_imm_immcope(num_args);

    codegen->virtual_reg_id++;
    ImmcOpe* dest = new_reg_immcope(codegen->virtual_reg_id);

    vector_push(codes, new_inst_immc(INST_CALL, dest, fst_src, snd_src));
    vector_push(codes, new_inst_immc(INST_CLEAN, NULL, new_imm_immcope(num_args), NULL));

    return codes;
}

Vector* gen_unary_expr_code(Codegen* codegen) {
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;
    Vector* gen_address_expr_code(Codegen* codegen);
    Vector* gen_not_expr_code(Codegen* codegen);

    switch (srt->type) {
        case SRT_ADDR_EXPR:
            codes = gen_address_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_not_expr_code(Codegen * codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* src = new_reg_immcope(codegen->virtual_reg_id);

    codegen->virtual_reg_id++;
    ImmcOpe* dest = new_reg_immcope(codegen->virtual_reg_id);

    switch (srt->type) {
        case SRT_LNOT_EXPR:
            vector_push(codes, new_inst_immc(INST_LNOT, dest, src, NULL));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_address_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = vector_at(codegen->_srt->children, 0);

    ImmcOpe* dest = NULL;
    ImmcOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immcope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immcope(string_copy(symbol->name));
                break;
            }
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->virtual_reg_id++;
    dest = new_reg_immcope(codegen->virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_ADDR, dest, src, NULL));

    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    ImmcOpe* dest = NULL;
    ImmcOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immcope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immcope(string_copy(symbol->name));
                break;
            }
            break;
        }
        case SRT_INT_EXPR:
            src = new_imm_immcope(srt->value_int);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->virtual_reg_id++;
    dest = new_reg_immcope(codegen->virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_LOAD, dest, src, NULL));

    return codes;
}
