#include "./statement.h"
#include "../common/type.h"
#include "../immc/immc.h"
#include "../pair/pair.h"
#include "./util.h"

#include <stdlib.h>

Vector* gen_compound_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_children_immcode(immcgen, codes);
    return codes;
}

Vector* gen_case_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Srt* case_value_srt = vector_at(immcgen->srt->children, 0);

    immcgen->label_id++;
    int case_label_id = immcgen->label_id;

    Pair* label_value_pair = new_pair(&t_integer, &t_iliteral);
    pair_set(label_value_pair, new_integer(case_label_id), iliteral_copy(case_value_srt->iliteral));
    vector_push(immcgen->case_label_values, label_value_pair);

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, case_label_id));

    append_child_immcode(immcgen, codes, 1);
    return codes;
}

Vector* gen_default_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    immcgen->label_id++;
    int default_label_id = immcgen->label_id;

    immcgen->default_label_id = default_label_id;

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, default_label_id));

    append_child_immcode(immcgen, codes, 0);

    return codes;
}

Vector* gen_continue_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    ImmcOpe* continue_label = new_label_immcope_from_id(immcgen->continue_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, continue_label, NULL, NULL));
    return codes;
}

Vector* gen_break_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    ImmcOpe* break_label = new_label_immcope_from_id(immcgen->break_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, break_label, NULL, NULL));
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

Vector* gen_switch_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    Vector* cases_codes = new_vector(&t_immc);
    ImmcOpe* reg = gen_child_reg_immcope(immcgen, cases_codes, 0);

    Vector* body_codes = new_vector(&t_immc);
    immcgen->label_id++;
    int break_label_id = immcgen->label_id;

    Vector* original_case_label_values = immcgen->case_label_values;
    int original_default_label_id = immcgen->default_label_id;
    int original_break_label_id = immcgen->break_label_id;

    immcgen->case_label_values = new_vector(&t_pair);
    immcgen->default_label_id = -1;

    immcgen->break_label_id = break_label_id;
    append_child_immcode(immcgen, body_codes, 1);
    immcgen->break_label_id = original_break_label_id;

    int num_labels = vector_size(immcgen->case_label_values);
    for (int i = 0; i < num_labels; i++) {
        Pair* label_value_pair = vector_at(immcgen->case_label_values, i);
        int* case_label_id_ref = pair_first(label_value_pair);
        ImmcOpe* case_label = new_label_immcope_from_id(*case_label_id_ref);

        IntegerLiteral* case_value_iliteral = pair_second(label_value_pair);
        ImmcSuffix suffix = immcsuffix_get(iliteral_nbytes(case_value_iliteral));
        ImmcOpe* case_value = new_int_immcope(suffix, iliteral_copy(case_value_iliteral));

        vector_push(cases_codes, new_inst_immc(IMMC_INST_JEQ, case_label, immcope_copy(reg), case_value));
    }
    if (immcgen->default_label_id > 0) {
        ImmcOpe* default_label = new_label_immcope_from_id(immcgen->default_label_id);
        vector_push(cases_codes, new_inst_immc(IMMC_INST_JMP, default_label, NULL, NULL));
    }
    delete_immcope(reg);

    delete_vector(immcgen->case_label_values);
    immcgen->case_label_values = original_case_label_values;
    immcgen->default_label_id = original_default_label_id;

    vector_push(body_codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, break_label_id));

    vector_extend(codes, cases_codes);
    delete_vector(cases_codes);
    vector_extend(codes, body_codes);
    delete_vector(body_codes);

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

    int original_continue_label_id = immcgen->continue_label_id;
    int original_break_label_id = immcgen->break_label_id;

    immcgen->continue_label_id = begin_label_id;
    immcgen->break_label_id = end_label_id;
    append_child_immcode(immcgen, codes, 1);
    immcgen->continue_label_id = original_continue_label_id;
    immcgen->break_label_id = original_break_label_id;

    ImmcOpe* begin_label = new_label_immcope_from_id(begin_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, begin_label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}

Vector* gen_for_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);

    immcgen->label_id++;
    int begin_label_id = immcgen->label_id;
    immcgen->label_id++;
    int continue_label_id = immcgen->label_id;
    immcgen->label_id++;
    int end_label_id = immcgen->label_id;

    append_child_immcode(immcgen, codes, 0);

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, begin_label_id));

    Srt* srt = immcgen->srt;
    Srt* controling_stmt_srt = vector_at(immcgen->srt->children, 1);
    if (controling_stmt_srt->type == SRT_EXPR_STMT) {
        ImmcOpe* end_label = new_label_immcope_from_id(end_label_id);
        immcgen->srt = controling_stmt_srt;
        append_child_jmp_false_immcode(immcgen, codes, 0, end_label);
        immcgen->srt = srt;
    }

    int original_continue_label_id = immcgen->continue_label_id;
    int original_break_label_id = immcgen->break_label_id;

    immcgen->continue_label_id = continue_label_id;
    immcgen->break_label_id = end_label_id;
    append_child_immcode(immcgen, codes, 3);
    immcgen->continue_label_id = original_continue_label_id;
    immcgen->break_label_id = original_break_label_id;

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, continue_label_id));

    append_child_immcode(immcgen, codes, 2);

    ImmcOpe* begin_label = new_label_immcope_from_id(begin_label_id);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, begin_label, NULL, NULL));

    vector_push(codes, new_label_immc_from_id(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, end_label_id));

    return codes;
}
