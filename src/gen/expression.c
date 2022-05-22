#include "./expression.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_assignment_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* dst = new_ptr_immcope(codegen->_virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* src = new_reg_immcope(codegen->_virtual_reg_id);

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            vector_push(codes, new_inst_immc(INST_STORE, dst, src, NULL));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_logical_or_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    codegen->_label_id++;
    char* true_label = create_label_name(codegen->_label_id);
    codegen->_label_id++;
    char* end_label = create_label_name(codegen->_label_id);

    append_child_code(codegen, codes, 0);

    dst = new_label_immcope(new_string(true_label));
    fst_src = new_reg_immcope(codegen->_virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(INST_JNEQ, dst, fst_src, snd_src));

    append_child_code(codegen, codes, 1);

    dst = new_label_immcope(new_string(true_label));
    fst_src = new_reg_immcope(codegen->_virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(INST_JNEQ, dst, fst_src, snd_src));

    codegen->_virtual_reg_id++;
    dst = new_reg_immcope(codegen->_virtual_reg_id);
    fst_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(INST_LOAD, dst, fst_src, NULL));

    dst = new_label_immcope(new_string(end_label));
    vector_push(codes, new_inst_immc(INST_JMP, dst, NULL, NULL));

    vector_push(codes, new_label_immc(LABEL_NORMAL, LABELVIS_DEFAULT, new_string(true_label)));

    dst = new_reg_immcope(codegen->_virtual_reg_id);
    fst_src = new_imm_immcope(1);
    vector_push(codes, new_inst_immc(INST_LOAD, dst, fst_src, NULL));

    vector_push(codes, new_label_immc(LABEL_NORMAL, LABELVIS_DEFAULT, new_string(end_label)));

    free(true_label);
    free(end_label);
    return codes;
}

Vector* gen_logical_and_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    codegen->_label_id++;
    char* false_label = create_label_name(codegen->_label_id);
    codegen->_label_id++;
    char* end_label = create_label_name(codegen->_label_id);

    append_child_code(codegen, codes, 0);

    dst = new_label_immcope(new_string(false_label));
    fst_src = new_reg_immcope(codegen->_virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(INST_JEQ, dst, fst_src, snd_src));

    append_child_code(codegen, codes, 1);

    dst = new_label_immcope(new_string(false_label));
    fst_src = new_reg_immcope(codegen->_virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(INST_JEQ, dst, fst_src, snd_src));

    codegen->_virtual_reg_id++;
    dst = new_reg_immcope(codegen->_virtual_reg_id);
    fst_src = new_imm_immcope(1);
    vector_push(codes, new_inst_immc(INST_LOAD, dst, fst_src, NULL));

    dst = new_label_immcope(new_string(end_label));
    vector_push(codes, new_inst_immc(INST_JMP, dst, NULL, NULL));

    vector_push(codes, new_label_immc(LABEL_NORMAL, LABELVIS_DEFAULT, new_string(false_label)));

    dst = new_reg_immcope(codegen->_virtual_reg_id);
    fst_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(INST_LOAD, dst, fst_src, NULL));

    vector_push(codes, new_label_immc(LABEL_NORMAL, LABELVIS_DEFAULT, new_string(end_label)));

    free(false_label);
    free(end_label);
    return codes;
}

Vector* gen_equality_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(codegen->_virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(codegen->_virtual_reg_id);

    codegen->_virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(codegen->_virtual_reg_id);

    switch (srt->type) {
        case SRT_EQUAL_EXPR:
            vector_push(codes, new_inst_immc(INST_SETEQ, dst, fst_src, snd_src));
            break;
        case SRT_NEQUAL_EXPR:
            vector_push(codes, new_inst_immc(INST_SETNEQ, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_additive_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(codegen->_virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(codegen->_virtual_reg_id);

    codegen->_virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(codegen->_virtual_reg_id);

    switch (srt->type) {
        case SRT_ADD_EXPR:
            vector_push(codes, new_inst_immc(INST_ADD, dst, fst_src, snd_src));
            break;
        case SRT_SUB_EXPR:
            vector_push(codes, new_inst_immc(INST_SUB, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_multiplicative_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(codegen->_virtual_reg_id);

    append_child_code(codegen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(codegen->_virtual_reg_id);

    codegen->_virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(codegen->_virtual_reg_id);

    switch (srt->type) {
        case SRT_MUL_EXPR:
            vector_push(codes, new_inst_immc(INST_MUL, dst, fst_src, snd_src));
            break;
        case SRT_DIV_EXPR:
            vector_push(codes, new_inst_immc(INST_DIV, dst, fst_src, snd_src));
            break;
        case SRT_MOD_EXPR:
            vector_push(codes, new_inst_immc(INST_MOD, dst, fst_src, snd_src));
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
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    Srt* param_srt = vector_at(srt->children, 1);
    int num_args = vector_size(param_srt->children);
    vector_push(codes, new_inst_immc(INST_PREP, NULL, new_imm_immcope(num_args), NULL));

    codegen->_srt = param_srt;
    for (int i = num_args - 1; i >= 0; i--) {
        append_child_code(codegen, codes, i);
        ImmcOpe* fst_src = new_imm_immcope(i);
        ImmcOpe* snd_src = new_reg_immcope(codegen->_virtual_reg_id);
        vector_push(codes, new_inst_immc(INST_STARG, NULL, fst_src, snd_src));
    }
    codegen->_srt = srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* fst_src = new_ptr_immcope(codegen->_virtual_reg_id);
    ImmcOpe* snd_src = new_imm_immcope(num_args);

    codegen->_virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(codegen->_virtual_reg_id);

    vector_push(codes, new_inst_immc(INST_CALL, dst, fst_src, snd_src));
    vector_push(codes, new_inst_immc(INST_CLEAN, NULL, new_imm_immcope(num_args), NULL));

    return codes;
}

Vector* gen_unary_expr_code(Codegen* codegen) {
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;
    Vector* gen_address_expr_code(Codegen * codegen);
    Vector* gen_indirection_expr_code(Codegen * codegen);
    Vector* gen_not_expr_code(Codegen * codegen);

    switch (srt->type) {
        case SRT_ADDR_EXPR:
            codes = gen_address_expr_code(codegen);
            break;
        case SRT_INDIR_EXPR:
            codes = gen_indirection_expr_code(codegen);
            break;
        case SRT_LNOT_EXPR:
            codes = gen_not_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_indirection_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);

    append_child_code(codegen, codes, 0);
    ImmcOpe* src = new_ptr_immcope(codegen->_virtual_reg_id);

    codegen->_virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(codegen->_virtual_reg_id);

    vector_push(codes, new_inst_immc(INST_LOAD, dst, src, NULL));

    return codes;
}

Vector* gen_address_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;
    Srt* child = vector_at(srt->children, 0);

    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    switch (child->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(codegen->_local_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_mem_immcope(symbol->memory_offset);
                break;
            }
            symbol = symboltable_search(codegen->_global_table, srt->ident_name);
            if (symbol != NULL) {
                src = new_label_immcope(new_string(symbol->name));
                break;
            }
            break;
        }
        case SRT_INDIR_EXPR:
            codegen->_srt = child;
            append_child_code(codegen, codes, 0);
            codegen->_srt = srt;
            src = new_reg_immcope(codegen->_virtual_reg_id);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    codegen->_virtual_reg_id++;
    dst = new_reg_immcope(codegen->_virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_ADDR, dst, src, NULL));

    return codes;
}

Vector* gen_not_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);
    ImmcOpe* src = new_reg_immcope(codegen->_virtual_reg_id);

    codegen->_virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(codegen->_virtual_reg_id);

    switch (srt->type) {
        case SRT_LNOT_EXPR:
            vector_push(codes, new_inst_immc(INST_SETEQ, dst, src, new_imm_immcope(0)));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_primary_expr_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    ImmcOpe* dst = NULL;
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
                src = new_label_immcope(new_string(symbol->name));
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

    codegen->_virtual_reg_id++;
    dst = new_reg_immcope(codegen->_virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_LOAD, dst, src, NULL));

    return codes;
}
