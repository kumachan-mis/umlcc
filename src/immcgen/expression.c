#include "./expression.h"
#include "../common/type.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_assignment_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* dst = gen_child_ptr_immcope(immcgen, codes, 0);
    ImmcOpe* src = gen_child_reg_immcope(immcgen, codes, 1);

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

    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    char* true_label = create_label_name(immcgen->label_id);
    immcgen->label_id++;
    char* end_label = create_label_name(immcgen->label_id);

    dst = new_label_immcope(new_string(true_label));
    fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    snd_src = new_signed_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JNEQ, dst, fst_src, snd_src));

    dst = new_label_immcope(new_string(true_label));
    fst_src = gen_child_reg_immcope(immcgen, codes, 1);
    snd_src = new_signed_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JNEQ, dst, fst_src, snd_src));

    dst = create_dest_reg_immcope(immcgen);
    fst_src = new_signed_immcope(dst->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    dst = new_label_immcope(new_string(end_label));
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, dst, NULL, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(true_label)));

    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    fst_src = new_signed_immcope(dst->suffix, INTEGER_INT, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(end_label)));

    free(true_label);
    free(end_label);
    return codes;
}

Vector* gen_logical_and_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    char* false_label = create_label_name(immcgen->label_id);
    immcgen->label_id++;
    char* end_label = create_label_name(immcgen->label_id);

    dst = new_label_immcope(new_string(false_label));
    fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    snd_src = new_signed_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, dst, fst_src, snd_src));

    dst = new_label_immcope(new_string(false_label));
    fst_src = gen_child_reg_immcope(immcgen, codes, 1);
    snd_src = new_signed_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, dst, fst_src, snd_src));

    dst = create_dest_reg_immcope(immcgen);
    fst_src = new_signed_immcope(dst->suffix, INTEGER_INT, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    dst = new_label_immcope(new_string(end_label));
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, dst, NULL, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(false_label)));

    dst = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    fst_src = new_signed_immcope(dst->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, fst_src, NULL));

    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(end_label)));

    free(false_label);
    free(end_label);
    return codes;
}

Vector* gen_equality_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_int_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

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

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_int_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_ADD_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));
            break;
        case SRT_SUB_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

Vector* gen_pointer_additive_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_reg_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_PADD_EXPR: {
            Srt* lhs_srt = vector_at(srt->children, 0);
            int mul_amount = dtype_size(lhs_srt->dtype->pointer->to_dtype);
            ImmcOpe* mul_fst_src = immcope_copy(snd_src);
            ImmcOpe* mul_snd_src = new_signed_immcope(mul_fst_src->suffix, INTEGER_INT, mul_amount);
            ImmcOpe* mul_dst = immcope_copy(snd_src);
            vector_push(codes, new_inst_immc(IMMC_INST_MUL, mul_dst, mul_fst_src, mul_snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));
            break;
        }
        case SRT_PSUB_EXPR: {
            Srt* lhs_srt = vector_at(srt->children, 0);
            int mul_amount = dtype_size(lhs_srt->dtype->pointer->to_dtype);
            ImmcOpe* mul_fst_src = immcope_copy(snd_src);
            ImmcOpe* mul_snd_src = new_signed_immcope(mul_fst_src->suffix, INTEGER_INT, mul_amount);
            ImmcOpe* mul_dst = immcope_copy(snd_src);
            vector_push(codes, new_inst_immc(IMMC_INST_MUL, mul_dst, mul_fst_src, mul_snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            break;
        }
        case SRT_PDIFF_EXPR: {
            Srt* lhs_srt = vector_at(srt->children, 0);
            int div_amount = dtype_size(lhs_srt->dtype->pointer->to_dtype);
            ImmcOpe* div_fst_src = immcope_copy(dst);
            ImmcOpe* div_snd_src = new_signed_immcope(div_fst_src->suffix, INTEGER_INT, div_amount);
            ImmcOpe* div_dst = immcope_copy(dst);
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_DIV, div_dst, div_fst_src, div_snd_src));
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

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_int_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

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

    Srt* param_srt = vector_at(srt->children, 1);
    int num_args = vector_size(param_srt->children);

    ImmcOpe* prep_src = new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, num_args);
    vector_push(codes, new_inst_immc(IMMC_INST_PREP, NULL, prep_src, NULL));

    immcgen->srt = param_srt;
    for (int i = num_args - 1; i >= 0; i--) {
        ImmcOpe* dst = new_arg_immcope(immcgen->virtual_reg_suffix, i);
        ImmcOpe* src = gen_child_int_immcope(immcgen, codes, i);
        vector_push(codes, new_inst_immc(IMMC_INST_STARG, dst, src, NULL));
    }
    immcgen->srt = srt;

    ImmcOpe* fst_src = gen_child_ptr_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, num_args);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_CALL, dst, fst_src, snd_src));

    ImmcOpe* clean_src = new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, num_args);
    vector_push(codes, new_inst_immc(IMMC_INST_CLEAN, NULL, clean_src, NULL));

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

    ImmcOpe* src = gen_child_ptr_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

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
            if (symbol == NULL && immcgen->local_table != NULL) {
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
            dst = create_dest_reg_immcope(immcgen);
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

    ImmcOpe* src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_LNOT_EXPR: {
            ImmcOpe* zero = new_signed_immcope(src->suffix, INTEGER_INT, 0);
            vector_push(codes, new_inst_immc(IMMC_INST_SETEQ, dst, src, zero));
            break;
        }
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
            if (symbol == NULL && immcgen->local_table != NULL) {
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
        case SRT_CHAR_EXPR:
            src = new_int_immcope(IMMC_SUFFIX_LONG, iliteral_copy(srt->iliteral));
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    dst = create_dest_reg_immcope(immcgen);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    return codes;
}
