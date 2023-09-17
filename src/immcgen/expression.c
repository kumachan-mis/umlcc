#include "./expression.h"
#include "../common/type.h"
#include "../immc/immc.h"
#include "../immc/util.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector* gen_assignment_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* src = gen_child_reg_immcope(immcgen, codes, 1);
    ImmcOpe* dst = gen_child_ptr_immcope(immcgen, codes, 0);

    switch (srt->type) {
        case SRT_ASSIGN_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_STORE, dst, src, NULL));
            break;
        default:
            fprintf(stderr, "unexpected srt type %d\n", srt->type);
            exit(1);
    }

    update_non_void_expr_register(immcgen, src);
    return codes;
}

Vector* gen_logical_or_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* reg = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    int true_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(true_label_id);
    fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    snd_src = new_signed_int_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JNEQ, label, fst_src, snd_src));

    label = new_label_immcope_from_id(true_label_id);
    fst_src = gen_child_reg_immcope(immcgen, codes, 1);
    snd_src = new_signed_int_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JNEQ, label, fst_src, snd_src));

    reg = create_dest_reg_immcope(immcgen);
    fst_src = new_signed_int_immcope(reg->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, reg, fst_src, NULL));

    label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, true_label_id));

    reg = immcope_copy(reg);
    fst_src = new_signed_int_immcope(reg->suffix, INTEGER_INT, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, reg, fst_src, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    update_non_void_expr_register(immcgen, reg);
    return codes;
}

Vector* gen_logical_and_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* reg = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    int false_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(false_label_id);
    fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    snd_src = new_signed_int_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, label, fst_src, snd_src));

    label = new_label_immcope_from_id(false_label_id);
    fst_src = gen_child_reg_immcope(immcgen, codes, 1);
    snd_src = new_signed_int_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, label, fst_src, snd_src));

    reg = create_dest_reg_immcope(immcgen);
    fst_src = new_signed_int_immcope(reg->suffix, INTEGER_INT, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, reg, fst_src, NULL));

    label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, false_label_id));

    reg = immcope_copy(reg);
    fst_src = new_signed_int_immcope(reg->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, reg, fst_src, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    update_non_void_expr_register(immcgen, reg);
    return codes;
}

Vector* gen_bitwise_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_int_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_AND_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_AND, dst, fst_src, snd_src));
            break;
        case SRT_OR_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_OR, dst, fst_src, snd_src));
            break;
        case SRT_XOR_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_XOR, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_bitwise_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
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
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_equality_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_relational_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_int_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_LESS_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETLT, dst, fst_src, snd_src));
            break;
        case SRT_GREATER_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETGT, dst, fst_src, snd_src));
            break;
        case SRT_LESSEQ_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETLEQ, dst, fst_src, snd_src));
            break;
        case SRT_GREATEREQ_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SETGEQ, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_relational_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
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
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_additive_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
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
            int nbytes = dtype_nbytes(lhs_srt->dtype->dpointer->to_dtype);
            ImmcOpe* mul_fst_src = immcope_copy(snd_src);
            ImmcOpe* mul_snd_src = new_signed_int_immcope(mul_fst_src->suffix, INTEGER_INT, nbytes);
            ImmcOpe* mul_dst = immcope_copy(snd_src);
            vector_push(codes, new_inst_immc(IMMC_INST_MUL, mul_dst, mul_fst_src, mul_snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));
            break;
        }
        case SRT_PSUB_EXPR: {
            Srt* lhs_srt = vector_at(srt->children, 0);
            int nbytes = dtype_nbytes(lhs_srt->dtype->dpointer->to_dtype);
            ImmcOpe* mul_fst_src = immcope_copy(snd_src);
            ImmcOpe* mul_snd_src = new_signed_int_immcope(mul_fst_src->suffix, INTEGER_INT, nbytes);
            ImmcOpe* mul_dst = immcope_copy(snd_src);
            vector_push(codes, new_inst_immc(IMMC_INST_MUL, mul_dst, mul_fst_src, mul_snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            break;
        }
        case SRT_PDIFF_EXPR: {
            Srt* lhs_srt = vector_at(srt->children, 0);
            int nbytes = dtype_nbytes(lhs_srt->dtype->dpointer->to_dtype);
            ImmcOpe* div_fst_src = immcope_copy(dst);
            ImmcOpe* div_snd_src = new_signed_int_immcope(div_fst_src->suffix, INTEGER_INT, nbytes);
            ImmcOpe* div_dst = immcope_copy(dst);
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            vector_push(codes, new_inst_immc(IMMC_INST_DIV, div_dst, div_fst_src, div_snd_src));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_pointer_additive_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
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
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_multiplicative_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_unary_expr_immcode(Immcgen* immcgen) {
    Vector* codes = NULL;
    Srt* srt = immcgen->srt;

    Vector* gen_address_expr_immcode(Immcgen * immcgen);
    Vector* gen_indirection_expr_immcode(Immcgen * immcgen);
    Vector* gen_positive_expr_immcode(Immcgen * immcgen);
    Vector* gen_negative_expr_immcode(Immcgen * immcgen);
    Vector* gen_not_expr_immcode(Immcgen * immcgen);
    Vector* gen_logical_not_expr_immcode(Immcgen * immcgen);

    switch (srt->type) {
        case SRT_ADDR_EXPR:
            codes = gen_address_expr_immcode(immcgen);
            break;
        case SRT_INDIR_EXPR:
            codes = gen_indirection_expr_immcode(immcgen);
            break;
        case SRT_POS_EXPR:
            codes = gen_positive_expr_immcode(immcgen);
            break;
        case SRT_NEG_EXPR:
            codes = gen_negative_expr_immcode(immcgen);
            break;
        case SRT_NOT_EXPR:
            codes = gen_not_expr_immcode(immcgen);
            break;
        case SRT_LNOT_EXPR:
            codes = gen_logical_not_expr_immcode(immcgen);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_unary_expr_immcode)\n");
            exit(1);
    }

    return codes;
}

Vector* gen_address_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, 0);

    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    switch (child_srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(immcgen->symbol_table, child_srt->ident_name);
            if (symbol->type == SYMBOL_LABEL) {
                src = new_label_immcope(new_string(symbol->name));
            } else {
                src = new_mem_immcope(symbol->memory_offset);
            }
            dst = create_dest_reg_immcope(immcgen);
            vector_push(codes, new_inst_immc(IMMC_INST_ADDR, dst, src, NULL));
            update_non_void_expr_register(immcgen, dst);
            break;
        }
        case SRT_STRIDENT_EXPR: {
            char* sliteral_label = create_sliteral_label(child_srt->sliteral_id);
            src = new_label_immcope(sliteral_label);
            dst = create_dest_reg_immcope(immcgen);
            vector_push(codes, new_inst_immc(IMMC_INST_ADDR, dst, src, NULL));
            update_non_void_expr_register(immcgen, dst);
            break;
        }
        case SRT_TOMEMBER_EXPR: {
            immcgen->srt = child_srt;
            src = gen_child_reg_immcope(immcgen, codes, 0);
            immcgen->srt = srt;

            Srt* struct_srt = vector_at(child_srt->children, 0);
            Srt* member_srt = vector_at(child_srt->children, 1);

            DType* struct_dtype = struct_srt->dtype->dpointer->to_dtype;
            if (struct_dtype->dstruct->members == NULL) {
                struct_dtype = tagtable_search(immcgen->tag_table, struct_dtype->dstruct->name);
            }

            DStructMember* accessed_member = NULL;
            int num_members = vector_size(struct_dtype->dstruct->members);
            for (int i = 0; i < num_members; i++) {
                DStructMember* member = vector_at(struct_dtype->dstruct->members, i);
                if (strcmp(member->name, member_srt->ident_name) == 0) {
                    accessed_member = member;
                    break;
                }
            }

            ImmcOpe* snd_src = new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, accessed_member->memory_offset);
            dst = create_dest_reg_immcope(immcgen);
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, src, snd_src));
            update_non_void_expr_register(immcgen, dst);
            break;
        }
        case SRT_INDIR_EXPR:
            immcgen->srt = child_srt;
            append_child_immcode(immcgen, codes, 0);
            immcgen->srt = srt;
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_address_expr_immcode)\n");
            exit(1);
    }

    return codes;
}

Vector* gen_indirection_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* src = gen_child_ptr_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_positive_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* dst = gen_child_reg_immcope(immcgen, codes, 0);

    update_non_void_expr_register(immcgen, dst);

    delete_immcope(dst);
    return codes;
}

Vector* gen_negative_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* src = gen_child_int_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_NEG, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_not_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* src = gen_child_int_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_NOT, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_logical_not_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_LNOT_EXPR: {
            ImmcOpe* zero = new_signed_int_immcope(src->suffix, INTEGER_INT, 0);
            vector_push(codes, new_inst_immc(IMMC_INST_SETEQ, dst, src, zero));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_not_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_postfix_expr_immcode(Immcgen* immcgen) {
    Vector* gen_call_expr_immcode(Immcgen * immcgen);
    Vector* gen_tomember_expr_immcode(Immcgen * immcgen);

    Vector* codes = NULL;
    Srt* srt = immcgen->srt;

    switch (srt->type) {
        case SRT_CALL_EXPR:
            codes = gen_call_expr_immcode(immcgen);
            break;
        case SRT_TOMEMBER_EXPR:
            codes = gen_tomember_expr_immcode(immcgen);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_postfix_expr_immcode)\n");
            exit(1);
    }

    return codes;
}

Vector* gen_call_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    Srt* param_srt = vector_at(srt->children, 1);
    int num_args = vector_size(param_srt->children);

    ImmcOpe* prep_src = new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, num_args);
    vector_push(codes, new_inst_immc(IMMC_INST_PREP, NULL, prep_src, NULL));

    immcgen->srt = param_srt;
    for (int i = num_args - 1; i >= 0; i--) {
        ImmcOpe* src = gen_child_int_immcope(immcgen, codes, i);
        ImmcOpe* dst = new_signed_arg_immcope(src->suffix, i);
        vector_push(codes, new_inst_immc(IMMC_INST_STARG, dst, src, NULL));
    }
    immcgen->srt = srt;

    ImmcOpe* fst_src = gen_child_ptr_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, num_args);

    ImmcOpe* dst = NULL;
    if (srt->dtype->type != DTYPE_VOID) {
        dst = create_dest_reg_immcope(immcgen);
    }

    vector_push(codes, new_inst_immc(IMMC_INST_CALL, dst, fst_src, snd_src));

    ImmcOpe* clean_src = new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, num_args);
    vector_push(codes, new_inst_immc(IMMC_INST_CLEAN, NULL, clean_src, NULL));

    if (srt->dtype->type != DTYPE_VOID) {
        update_non_void_expr_register(immcgen, dst);
    } else {
        update_void_expr_register(immcgen);
    }
    return codes;
}

Vector* gen_tomember_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* add_fst_src = gen_child_reg_immcope(immcgen, codes, 0);

    Srt* struct_srt = vector_at(srt->children, 0);
    Srt* member_srt = vector_at(srt->children, 1);

    DType* struct_dtype = struct_srt->dtype->dpointer->to_dtype;
    if (struct_dtype->dstruct->members == NULL) {
        struct_dtype = tagtable_search(immcgen->tag_table, struct_dtype->dstruct->name);
    }

    DStructMember* accessed_member = NULL;
    int num_members = vector_size(struct_dtype->dstruct->members);
    for (int i = 0; i < num_members; i++) {
        DStructMember* member = vector_at(struct_dtype->dstruct->members, i);
        if (strcmp(member->name, member_srt->ident_name) == 0) {
            accessed_member = member;
            break;
        }
    }

    ImmcOpe* add_snd_src = new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, accessed_member->memory_offset);
    immcgen->next_reg_id++;
    ImmcOpe* add_dst = new_signed_reg_immcope(IMMC_SUFFIX_QUAD, immcgen->next_reg_id);
    vector_push(codes, new_inst_immc(IMMC_INST_ADD, add_dst, add_fst_src, add_snd_src));

    ImmcOpe* src = new_ptr_immcope(add_dst->reg->reg_id);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_primary_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    switch (srt->type) {
        case SRT_IDENT_EXPR: {
            Symbol* symbol = symboltable_search(immcgen->symbol_table, srt->ident_name);
            if (symbol->type == SYMBOL_LABEL) {
                src = new_label_immcope(new_string(symbol->name));
            } else {
                src = new_mem_immcope(symbol->memory_offset);
            }
            break;
        }
        case SRT_STRIDENT_EXPR: {
            char* sliteral_label = create_sliteral_label(srt->sliteral_id);
            src = new_label_immcope(sliteral_label);
            break;
        }
        case SRT_INT_EXPR:
        case SRT_CHAR_EXPR: {
            ImmcSuffix suffix = immcsuffix_get(dtype_nbytes(srt->dtype));
            src = new_int_immcope(suffix, iliteral_copy(srt->iliteral));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_primary_expr_immcode)\n");
            exit(1);
    }

    dst = create_dest_reg_immcope(immcgen);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}
