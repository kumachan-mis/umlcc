#include "./statement.h"
#include "../common/type.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>

Vector* gen_compound_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_children_immcode(immcgen, codes);
    return codes;
}

Vector* gen_return_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    if (vector_size(srt->children) > 0) {
        ImmcOpe* ret_value = gen_child_int_immcope(immcgen, codes, 0);
        vector_push(codes, new_inst_immc(IMMC_INST_STRET, NULL, ret_value, NULL));
    }

    ImmcOpe* ret_label = new_label_immcope_from_id(immcgen->return_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, ret_label, NULL, NULL));

    immcgen->srt = srt;
    return codes;
}

Vector* gen_expression_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    immcgen->expr_reg_suffix = IMMC_SUFFIX_NONE;
    if (immcgen->expr_reg != NULL) {
        delete_immcreg(immcgen->expr_reg);
        immcgen->expr_reg = NULL;
    }
    append_child_immcode(immcgen, codes, 0);
    return codes;
}

Vector* gen_if_else_stmt_immcode(Immcgen* immcgen) {
    Vector* gen_if_without_else_stmt_immcode(Immcgen * immcgen);
    Vector* gen_if_with_else_stmt_immcode(Immcgen * immcgen);

    Srt* srt = immcgen->srt;

    if (vector_size(srt->children) == 2) {
        return gen_if_without_else_stmt_immcode(immcgen);
    } else {
        return gen_if_with_else_stmt_immcode(immcgen);
    }
}

Vector* gen_if_without_else_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(end_label_id);
    fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    snd_src = new_signed_int_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, label, fst_src, snd_src));

    append_child_immcode(immcgen, codes, 1);

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}

Vector* gen_if_with_else_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    ImmcOpe* label = NULL;
    ImmcOpe* fst_src = NULL;
    ImmcOpe* snd_src = NULL;

    immcgen->label_id++;
    int else_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    label = new_label_immcope_from_id(else_label_id);
    fst_src = gen_child_reg_immcope(immcgen, codes, 0);
    snd_src = new_signed_int_immcope(fst_src->suffix, INTEGER_INT, 0);
    vector_push(codes, new_inst_immc(IMMC_INST_JEQ, label, fst_src, snd_src));

    append_child_immcode(immcgen, codes, 1);

    label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, else_label_id));

    append_child_immcode(immcgen, codes, 2);

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}
