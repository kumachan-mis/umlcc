#include "./statement.h"
#include "../immc/immc.h"
#include "./util.h"
#include "../common/common.h"

#include <stdlib.h>

Vector* gen_compound_stmt_code(Codegen* codegen) {
    return gen_children_code(codegen);
}

Vector* gen_return_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);

    ImmcOpe* ret_value = new_reg_immcope(codegen->virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_STRET, NULL, ret_value, NULL));

    ImmcOpe* ret_label = new_label_immcope(string_copy( codegen->return_label));
    vector_push(codes, new_inst_immc(INST_JMP, NULL, ret_label, NULL));

    codegen->_srt = srt;
    return codes;
}

Vector* gen_expression_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);

    ImmcOpe* src = new_reg_immcope(codegen->virtual_reg_id);
    vector_push(codes, new_inst_immc(INST_FREE, NULL, src, NULL));

    codegen->_srt = srt;
    return codes;
}
