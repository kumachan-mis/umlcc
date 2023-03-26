#include "./utils.h"

int ast_is_void_parameter_list(Ast* ast) {
    if (vector_size(ast->children) != 1) return 0;

    Ast* param_decl_ast = vector_at(ast->children, 0);

    Ast* param_decl_specs_ast = vector_at(param_decl_ast->children, 0);
    if (vector_size(param_decl_specs_ast->children) != 1) return 0;

    Ast* param_decl_spec_ast = vector_at(param_decl_specs_ast->children, 0);

    Ast* terminal_ast = vector_at(param_decl_ast->children, 1);

    return param_decl_spec_ast->type == AST_TYPE_VOID && terminal_ast->type == AST_ABS_DECLOR;
}
