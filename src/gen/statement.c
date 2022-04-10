#include "./gen.h"
#include "./util.h"

#include <stdlib.h>


Vector* gen_expression_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Ast* ast = codegen->_ast;

    codegen->_ast = vector_at(ast->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    append_code(codes, "    popq %%rax\n");

    codegen->_ast = ast;
    return codes;
}
