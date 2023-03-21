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

    char* label_name = create_label_name(immcgen->return_label_id);
    ImmcOpe* ret_label = new_label_immcope(label_name);
    vector_push(codes, new_inst_immc(IMMC_INST_JMP, ret_label, NULL, NULL));

    immcgen->srt = srt;
    return codes;
}

Vector* gen_expression_stmt_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    immcgen->expr_reg_suffix = IMMC_SUFFIX_NONE;
    immcgen->expr_reg_id = -1;
    append_child_immcode(immcgen, codes, 0);
    return codes;
}
