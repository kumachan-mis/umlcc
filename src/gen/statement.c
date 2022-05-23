#include "./statement.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>

Vector* gen_compound_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    append_children_code(codegen, codes);
    return codes;
}

Vector* gen_return_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = codegen->_srt;

    append_child_code(codegen, codes, 0);

    ImmcOpe* ret_value = new_reg_immcope(codegen->_virtual_reg_suffix, codegen->_virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_STRET, NULL, ret_value, NULL));

    char* label_name = create_label_name(codegen->_return_label_id);
    ImmcOpe* ret_label = new_label_immcope(label_name);
    vector_push(codes, new_inst_immc(INST_JMP, ret_label, NULL, NULL));

    codegen->_srt = srt;
    return codes;
}

Vector* gen_expression_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    append_child_code(codegen, codes, 0);
    return codes;
}
