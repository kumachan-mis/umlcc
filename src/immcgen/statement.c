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

Vector* gen_null_stmt_immcode(void) {
    return new_vector(&t_immc);
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

    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    ImmcOpe* end_label = new_label_immcope_from_id(end_label_id);
    append_child_jmp_false_immcode(immcgen, codes, 0, end_label);

    append_child_immcode(immcgen, codes, 1);

    Immc* end_code = vector_at(codes, vector_size(codes) - 1);
    if (end_code->type == IMMC_LABEL) {
        // reuse label created by child
        free(end_label->label_name);
        end_label->label_name = new_string(end_code->label->name);
        return codes;
    }

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}

Vector* gen_if_with_else_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    immcgen->label_id++;
    int else_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    ImmcOpe* else_label = new_label_immcope_from_id(else_label_id);
    append_child_jmp_false_immcode(immcgen, codes, 0, else_label);

    append_child_immcode(immcgen, codes, 1);

    ImmcOpe* end_label = new_label_immcope_from_id(end_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, end_label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, else_label_id));

    append_child_immcode(immcgen, codes, 2);

    Immc* end_code = vector_at(codes, vector_size(codes) - 1);
    if (end_code->type == IMMC_LABEL) {
        // reuse label created by child
        free(end_label->label_name);
        end_label->label_name = new_string(end_code->label->name);
        return codes;
    }

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}

Vector* gen_while_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    immcgen->label_id++;
    int begin_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, begin_label_id));

    ImmcOpe* end_label = new_label_immcope_from_id(end_label_id);
    append_child_jmp_false_immcode(immcgen, codes, 0, end_label);

    append_child_immcode(immcgen, codes, 1);

    ImmcOpe* begin_label = new_label_immcope_from_id(begin_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, begin_label, NULL, NULL));

    Immc* end_code = vector_at(codes, vector_size(codes) - 1);
    if (end_code->type == IMMC_LABEL) {
        // reuse label created by child
        free(end_label->label_name);
        end_label->label_name = new_string(end_code->label->name);
        return codes;
    }

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}
