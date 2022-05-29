#include "./expression.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_assignment_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* dst = new_ptr_immcope(immcgen->virtual_reg_id);

    append_child_immcode(immcgen, codes, 1);
    ImmcOpe* src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_STORE, dst, src, NULL));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_logical_or_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    char* true_label = create_label_name(immcgen->label_id);
    immcgen->label_id++;
    char* end_label = create_label_name(immcgen->label_id);

    append_child_immcode(immcgen, codes, 0);

    dst = new_label_immcope(new_string(true_label));
    fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(IMMC_INST_JNEQ, dst, fst_src, snd_src));

    append_child_immcode(immcgen, codes, 1);

    dst = new_label_immcope(new_string(true_label));
    fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(IMMC_INST_JNEQ, dst, fst_src, snd_src));

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    fst_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    dst = new_label_immcope(new_string(end_label));
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, dst, NULL, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(true_label)));

    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    fst_src = new_imm_immcope(1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(end_label)));

    free(true_label);
    free(end_label);
    return codes;
}

Vector* gen_logical_and_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    char* false_label = create_label_name(immcgen->label_id);
    immcgen->label_id++;
    char* end_label = create_label_name(immcgen->label_id);

    append_child_immcode(immcgen, codes, 0);

    dst = new_label_immcope(new_string(false_label));
    fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, dst, fst_src, snd_src));

    append_child_immcode(immcgen, codes, 1);

    dst = new_label_immcope(new_string(false_label));
    fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    snd_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, dst, fst_src, snd_src));

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    fst_src = new_imm_immcope(1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    dst = new_label_immcope(new_string(end_label));
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, dst, NULL, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(false_label)));

    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    fst_src = new_imm_immcope(0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(end_label)));

    free(false_label);
    free(end_label);
    return codes;
}

Vector* gen_equality_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    append_child_immcode(immcgen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    switch (srt->type) {
        case SRT_EQUAL_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETEQ, dst, fst_src, snd_src));
            break;
        case SRT_NEQUAL_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETNEQ, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_additive_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    append_child_immcode(immcgen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    switch (srt->type) {
        case SRT_ADD_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));
            break;
        case SRT_SUB_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            break;
        case SRT_PADD_EXPR: {
            Srt* lhs = vector_at(srt->children, 0);
            int shift_amount = dtype_log2_size(lhs->dtype->pointer->to_dtype);
            ImmcOpe* sal_fst_src = immcope_copy(snd_src);
            ImmcOpe* sal_snd_src = new_imm_immcope(shift_amount);
            ImmcOpe* sal_dst = immcope_copy(snd_src);
            vector_push(codes, new_inst_immc(IMMC_INST_SAL, sal_dst, sal_fst_src, sal_snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));
            break;
        }
        case SRT_PSUB_EXPR: {
            Srt* lhs = vector_at(srt->children, 0);
            int shift_amount = dtype_log2_size(lhs->dtype->pointer->to_dtype);
            ImmcOpe* sal_fst_src = immcope_copy(snd_src);
            ImmcOpe* sal_snd_src = new_imm_immcope(shift_amount);
            ImmcOpe* sal_dst = immcope_copy(snd_src);
            vector_push(codes, new_inst_immc(IMMC_INST_SAL, sal_dst, sal_fst_src, sal_snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            break;
        }
        case SRT_PDIFF_EXPR: {
            Srt* lhs = vector_at(srt->children, 0);
            int shift_amount = dtype_log2_size(lhs->dtype->pointer->to_dtype);
            ImmcOpe* sar_fst_src = immcope_copy(dst);
            ImmcOpe* sar_snd_src = new_imm_immcope(shift_amount);
            ImmcOpe* sar_dst = immcope_copy(dst);
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_SAR, sar_dst, sar_fst_src, sar_snd_src));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_multiplicative_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* fst_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    append_child_immcode(immcgen, codes, 1);
    ImmcOpe* snd_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    switch (srt->type) {
        case SRT_MUL_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_MUL, dst, fst_src, snd_src));
            break;
        case SRT_DIV_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_DIV, dst, fst_src, snd_src));
            break;
        case SRT_MOD_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_MOD, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_postfix_expr_immcode(Immcgen* immcgen) {
    Vector* codes = NULL;
    Srt* srt = immcgen->srt;
    Vector* gen_call_expr_immcode(Immcgen * immcgen);

    switch (srt->type) {
        case SRT_CALL_EXPR:
            codes = gen_call_expr_immcode(immcgen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_call_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    Srt* param = vector_at(srt->children, 1);
    int num_args = vector_size(param->children);
    vector_push(codes, new_inst_immc(IMMC_INST_PREP, NULL, new_imm_immcope(num_args), NULL));

    immcgen->srt = param;
    for (int i = num_args - 1; i >= 0; i--) {
        append_child_immcode(immcgen, codes, i);
        ImmcOpe* fst_src = new_arg_immcope(immcgen->virtual_reg_suffix, i);
        ImmcOpe* snd_src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
        vector_push(codes, new_inst_immc(IMMC_INST_STARG, NULL, fst_src, snd_src));
    }
    immcgen->srt = srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* fst_src = new_ptr_immcope(immcgen->virtual_reg_id);
    ImmcOpe* snd_src = new_imm_immcope(num_args);

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    vector_push(codes, new_inst_immc(IMMC_INST_CALL, dst, fst_src, snd_src));
    vector_push(codes, new_inst_immc(IMMC_INST_CLEAN, NULL, new_imm_immcope(num_args), NULL));

    return codes;
}

Vector* gen_unary_expr_immcode(Immcgen* immcgen) {
    Vector* codes = NULL;
    Srt* srt = immcgen->srt;
    Vector* gen_address_expr_immcode(Immcgen * immcgen);
    Vector* gen_indirection_expr_immcode(Immcgen * immcgen);
    Vector* gen_not_expr_immcode(Immcgen * immcgen);

    switch (srt->type) {
        case SRT_ADDR_EXPR:
            codes = gen_address_expr_immcode(immcgen);
            break;
        case SRT_INDIR_EXPR:
            codes = gen_indirection_expr_immcode(immcgen);
            break;
        case SRT_LNOT_EXPR:
            codes = gen_not_expr_immcode(immcgen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_indirection_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* src = new_ptr_immcope(immcgen->virtual_reg_id);

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    return codes;
}

Vector* gen_address_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;
    Srt* child = vector_at(srt->children, 0);

    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    switch (child->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = NULL;
            if (symbol == NULL) {
                symbol = symboltable_search(immcgen->local_table, child->ident_name);
            }
            if (symbol == NULL) {
                symbol = symboltable_search(immcgen->global_table, child->ident_name);
            }
            if (symbol->type == SYMBOL_LABEL) {
                src = new_label_immcope(new_string(symbol->name));
            } else {
                src = new_mem_immcope(symbol->memory_offset);
            }
            immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
            immcgen->virtual_reg_id++;
            dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
            vector_push(codes, new_inst_immc(IMMC_INST_ADDR, dst, src, NULL));
            break;
        }
        case SRT_INDIR_EXPR:
            immcgen->srt = child;
            append_child_immcode(immcgen, codes, 0);
            immcgen->srt = srt;
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_not_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    append_child_immcode(immcgen, codes, 0);
    ImmcOpe* src = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    ImmcOpe* dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);

    switch (srt->type) {
        case SRT_LNOT_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETEQ, dst, src, new_imm_immcope(0)));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_primary_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = NULL;
            if (symbol == NULL) {
                symbol = symboltable_search(immcgen->local_table, srt->ident_name);
            }
            if (symbol == NULL) {
                symbol = symboltable_search(immcgen->global_table, srt->ident_name);
            }
            if (symbol->type == SYMBOL_LABEL) {
                src = new_label_immcope(new_string(symbol->name));
            } else {
                src = new_mem_immcope(symbol->memory_offset);
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

    immcgen->virtual_reg_suffix = immcsuffix_get(dtype_size(srt->dtype));
    immcgen->virtual_reg_id++;
    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    return codes;
}
