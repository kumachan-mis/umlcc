#include "./statement.h"
#include "./util.h"

#include <stdlib.h>


Vector* gen_compound_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;
    codegen->_local_table = symboltable_enter_scope(codegen->_local_table);

    int num_children = vector_size(srt->children);
    for (int i = 0; i < num_children; i++) {
        codegen->_srt = vector_at(srt->children, i);
        Vector* sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    codegen->_local_table = symboltable_exit_scope(codegen->_local_table);
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
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rax\n");

    codegen->_srt = srt;
    return codes;
}
