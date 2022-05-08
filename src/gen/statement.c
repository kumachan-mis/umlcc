#include "./statement.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>

Vector* gen_compound_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    append_children_code(codegen, codes);
    return codes;
}

Vector* gen_return_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);

    ImmcOpe* ret_value = new_reg_immcope(codegen->virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_STRET, NULL, ret_value, NULL));

    ImmcOpe* ret_label = new_label_immcope(string_copy(codegen->return_label));
    vector_push(codes, new_inst_immc(INST_JMP, ret_label, NULL, NULL));

    codegen->_srt = srt;
    return codes;
}

Vector* gen_expression_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);

    codegen->_srt = srt;
    return codes;
}
