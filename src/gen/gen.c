#include "./gen.h"
#include "./declaration.h"
#include "./statement.h"
#include "./expression.h"

#include <stdlib.h>


Codegen* new_codegen(Ast* ast) {
    Codegen* codegen = malloc(sizeof(Codegen));
    codegen->_ast = ast;
    codegen->_table = new_symboltable();
    codegen->_assignee_mode = 0;
    return codegen;
}

void delete_codegen(Codegen* codegen) {
    delete_ast(codegen->_ast);
    delete_symboltable(codegen->_table);
    free(codegen);
}

Vector* codegen_generate_code(Codegen* codegen) {
    Vector* codes = NULL;
    Ast* ast = codegen->_ast;

    switch (ast->type) {
        case AST_DECL:
            codes = gen_decl_code(codegen);
            break;
        case AST_CMPD_STMT:
            codes = gen_compound_stmt_code(codegen);
            break;
        case AST_EXPR_STMT:
            codes = gen_expression_stmt_code(codegen);
            break;
        case AST_ASSIGN_EXPR:
            codes = gen_assignment_expr_code(codegen);
            break;
        case AST_ADD_EXPR:
        case AST_SUB_EXPR:
            codes = gen_additive_expr_code(codegen);
            break;
        case AST_MUL_EXPR:
        case AST_DIV_EXPR:
        case AST_MOD_EXPR:
            codes = gen_multiplicative_expr_code(codegen);
            break;
        case AST_IDENT:
        case AST_INT:
            if (!codegen->_assignee_mode) codes = gen_primary_expr_code(codegen);
            else codes = gen_assignee_primary_expr_code(codegen);
            break;
        default:
            break;
    }

    return codes;
}
