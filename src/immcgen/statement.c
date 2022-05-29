#include "./statement.h"
#include "../common/common.h"
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

    append_child_immcode(immcgen, codes, 0);

    ImmcOpe* ret_value = new_reg_immcope(immcgen->virtual_reg_suffix, immcgen->virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_STRET, NULL, ret_value, NULL));

    char* label_name = create_label_name(immcgen->return_label_id);
    ImmcOpe* ret_label = new_label_immcope(label_name);
    vector_push(codes, new_inst_immc(INST_JMP, ret_label, NULL, NULL));

    immcgen->srt = srt;
    return codes;
}

Vector* gen_expression_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_child_immcode(immcgen, codes, 0);
    return codes;
}
