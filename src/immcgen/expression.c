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

    ImmcOpe* src = gen_child_reg_immcope(immcgen, codes, 1);
    ImmcOpe* dst = gen_child_ptr_immcope(immcgen, codes, 0);

    vector_push(codes, new_inst_immc(IMMC_INST_STORE, dst, src, NULL));

    update_non_void_expr_register(immcgen, src);
    return codes;
}

Vector* gen_conditional_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    immcgen->label_id++;
    int false_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(false_label_id);
    append_child_jmp_false_immcode(immcgen, codes, 0, label);

    dst = create_dest_reg_immcope(immcgen);
    src = gen_child_reg_immcope(immcgen, codes, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, false_label_id));

    dst = immcope_copy(dst);
    src = gen_child_reg_immcope(immcgen, codes, 2);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_logical_or_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    immcgen->label_id++;
    int true_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(true_label_id);
    append_child_jmp_true_immcode(immcgen, codes, 0, label);

    label = new_label_immcope_from_id(true_label_id);
    append_child_jmp_true_immcode(immcgen, codes, 1, label);

    dst = create_dest_reg_immcope(immcgen);
    src = new_signed_int_immcope(dst->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, true_label_id));

    dst = immcope_copy(dst);
    src = new_signed_int_immcope(dst->suffix, INTEGER_INT, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_logical_and_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    immcgen->label_id++;
    int false_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(false_label_id);
    append_child_jmp_false_immcode(immcgen, codes, 0, label);

    label = new_label_immcope_from_id(false_label_id);
    append_child_jmp_false_immcode(immcgen, codes, 1, label);

    dst = create_dest_reg_immcope(immcgen);
    src = new_signed_int_immcope(dst->suffix, INTEGER_INT, 1);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, false_label_id));

    dst = immcope_copy(dst);
    src = new_signed_int_immcope(dst->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    update_non_void_expr_register(immcgen, dst);
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

Vector* gen_shift_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_int_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    switch (srt->type) {
        case SRT_LSHIFT_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SAL, dst, fst_src, snd_src));
            break;
        case SRT_RSHIFT_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SAR, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_shift_expr_immcode)\n");
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

    Srt* lhs_srt = vector_at(srt->children, 0);
    int nbytes = dtype_nbytes(lhs_srt->dtype->dpointer->to_dtype);

    ImmcOpe* mul_fst_src = immcope_copy(snd_src);
    ImmcOpe* mul_snd_src = new_signed_int_immcope(mul_fst_src->suffix, INTEGER_INT, nbytes);
    ImmcOpe* mul_dst = immcope_copy(snd_src);

    vector_push(codes, new_inst_immc(IMMC_INST_MUL, mul_dst, mul_fst_src, mul_snd_src));

    switch (srt->type) {
        case SRT_PADD_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));
            break;
        case SRT_PSUB_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_pointer_additive_expr_immcode)\n");
            exit(1);
    }

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_pointer_diff_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* snd_src = gen_child_reg_immcope(immcgen, codes, 1);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    Srt* lhs_srt = vector_at(srt->children, 0);
    int nbytes = dtype_nbytes(lhs_srt->dtype->dpointer->to_dtype);

    ImmcOpe* div_fst_src = immcope_copy(dst);
    ImmcOpe* div_snd_src = new_signed_int_immcope(div_fst_src->suffix, INTEGER_INT, nbytes);
    ImmcOpe* div_dst = immcope_copy(dst);

    vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, fst_src, snd_src));

    vector_push(codes, new_inst_immc(IMMC_INST_DIV, div_dst, div_fst_src, div_snd_src));

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

    Vector* gen_incdec_expr_immcode(Immcgen * immcgen);
    Vector* gen_address_expr_immcode(Immcgen * immcgen);
    Vector* gen_indirection_expr_immcode(Immcgen * immcgen);
    Vector* gen_positive_expr_immcode(Immcgen * immcgen);
    Vector* gen_negative_expr_immcode(Immcgen * immcgen);
    Vector* gen_not_expr_immcode(Immcgen * immcgen);
    Vector* gen_logical_not_expr_immcode(Immcgen * immcgen);

    switch (srt->type) {
        case SRT_PREINC_EXPR:
        case SRT_PREDEC_EXPR:
            codes = gen_incdec_expr_immcode(immcgen);
            break;
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
    Vector* gen_identifier_address_expr_immcode(Immcgen * immcgen);
    Vector* gen_string_address_expr_immcode(Immcgen * immcgen);
    Vector* gen_tomember_address_expr_immcode(Immcgen * immcgen);
    Vector* gen_indirection_address_expr_immcode(Immcgen * immcgen);

    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, 0);
    switch (child_srt->type) {
        case SRT_IDENT_EXPR:
            return gen_identifier_address_expr_immcode(immcgen);
        case SRT_STR_EXPR:
            return gen_string_address_expr_immcode(immcgen);
        case SRT_TOMEMBER_EXPR:
            return gen_tomember_address_expr_immcode(immcgen);
        case SRT_INDIR_EXPR:
            return gen_indirection_address_expr_immcode(immcgen);
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_address_expr_immcode)\n");
            exit(1);
    }
}

Vector* gen_identifier_address_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, 0);

    ImmcOpe* dst = NULL;
    ImmcOpe* src = NULL;

    Symbol* symbol = symboltable_search(immcgen->symbol_table, child_srt->ident_name);
    if (symbol->type == SYMBOL_LABEL) {
        src = new_label_immcope(new_string(symbol->name));
    } else {
        src = new_mem_immcope(symbol->memory_offset);
    }

    dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_ADDR, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_string_address_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, 0);

    char* sliteral_label = create_sliteral_label(child_srt->sliteral_id);
    ImmcOpe* src = new_label_immcope(sliteral_label);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);
    vector_push(codes, new_inst_immc(IMMC_INST_ADDR, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_tomember_address_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, 0);

    immcgen->srt = child_srt;
    ImmcOpe* fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    immcgen->srt = srt;

    Srt* struct_srt = vector_at(child_srt->children, 0);
    DType* struct_dtype = struct_srt->dtype->dpointer->to_dtype;
    if (struct_dtype->dstruct->members == NULL) {
        struct_dtype = tagtable_search(immcgen->tag_table, struct_dtype->dstruct->name);
    }

    DStructMember* accessed_member = NULL;
    Srt* member_srt = vector_at(child_srt->children, 1);
    int num_members = vector_size(struct_dtype->dstruct->members);
    for (int i = 0; i < num_members; i++) {
        DStructMember* member = vector_at(struct_dtype->dstruct->members, i);
        if (strcmp(member->name, member_srt->ident_name) == 0) {
            accessed_member = member;
            break;
        }
    }

    ImmcOpe* snd_src = new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, accessed_member->memory_offset);

    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, fst_src, snd_src));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_indirection_address_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, 0);

    immcgen->srt = child_srt;
    append_child_immcode(immcgen, codes, 0);
    immcgen->srt = srt;

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

    ImmcOpe* src = gen_child_reg_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);
    ImmcOpe* zero = new_signed_int_immcope(src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_SETEQ, dst, src, zero));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_postfix_expr_immcode(Immcgen* immcgen) {
    Vector* gen_call_expr_immcode(Immcgen * immcgen);
    Vector* gen_tomember_expr_immcode(Immcgen * immcgen);
    Vector* gen_incdec_expr_immcode(Immcgen * immcgen);

    Vector* codes = NULL;
    Srt* srt = immcgen->srt;

    switch (srt->type) {
        case SRT_CALL_EXPR:
            codes = gen_call_expr_immcode(immcgen);
            break;
        case SRT_TOMEMBER_EXPR:
            codes = gen_tomember_expr_immcode(immcgen);
            break;
        case SRT_POSTINC_EXPR:
        case SRT_POSTDEC_EXPR:
            codes = gen_incdec_expr_immcode(immcgen);
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
    Vector* codes = NULL;
    Srt* srt = immcgen->srt;

    immcgen->srt = new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(srt->dtype)), 1, srt_copy(srt));
    codes = gen_tomember_address_expr_immcode(immcgen);
    delete_srt(immcgen->srt);
    immcgen->srt = srt;

    ImmcOpe* src = new_ptr_immcope(immcgen->expr_reg->reg_id);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);
    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));

    update_non_void_expr_register(immcgen, dst);
    return codes;
}

Vector* gen_incdec_expr_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* ptr_src = gen_child_ptr_immcope(immcgen, codes, 0);
    ImmcOpe* dst = create_dest_reg_immcope(immcgen);

    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, ptr_src, NULL));

    ImmcOpe* reg_src = immcope_copy(dst);
    ImmcOpe* int_src = NULL;
    if (dtype_issignedinteger(immcgen->srt->dtype)) {
        int_src = new_signed_int_immcope(dst->suffix, INTEGER_INT, 1);
    } else if (dtype_isunsignedinteger(immcgen->srt->dtype)) {
        int_src = new_unsigned_int_immcope(dst->suffix, INTEGER_UNSIGNED_INT, 1u);
    } else {
        int pointer_nbytes = dtype_nbytes(immcgen->srt->dtype->dpointer->to_dtype);
        int incdec_value = pointer_nbytes > 0 ? pointer_nbytes : 1;
        int_src = new_signed_int_immcope(dst->suffix, INTEGER_INT, incdec_value);
    }
    dst = immcope_copy(dst);

    ImmcOpe* expr_reg = NULL;
    switch (immcgen->srt->type) {
        case SRT_PREINC_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, reg_src, int_src));
            expr_reg = dst;
            break;
        case SRT_PREDEC_EXPR:
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, reg_src, int_src));
            expr_reg = dst;
            break;
        case SRT_POSTINC_EXPR:
            expr_reg = create_dest_reg_immcope(immcgen);
            vector_push(codes, new_inst_immc(IMMC_INST_LOAD, expr_reg, dst, NULL));
            dst = immcope_copy(dst);
            vector_push(codes, new_inst_immc(IMMC_INST_ADD, dst, reg_src, int_src));
            break;
        case SRT_POSTDEC_EXPR:
            expr_reg = create_dest_reg_immcope(immcgen);
            vector_push(codes, new_inst_immc(IMMC_INST_LOAD, expr_reg, dst, NULL));
            dst = immcope_copy(dst);
            vector_push(codes, new_inst_immc(IMMC_INST_SUB, dst, reg_src, int_src));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_incdec_expr_immcode)\n");
            exit(1);
    }

    dst = immcope_copy(dst);
    ptr_src = immcope_copy(ptr_src);
    vector_push(codes, new_inst_immc(IMMC_INST_STORE, ptr_src, dst, NULL));

    update_non_void_expr_register(immcgen, expr_reg);
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
        case SRT_STR_EXPR: {
            char* sliteral_label = create_sliteral_label(srt->sliteral_id);
            src = new_label_immcope(sliteral_label);
            break;
        }
        case SRT_ILITERAL_EXPR: {
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
