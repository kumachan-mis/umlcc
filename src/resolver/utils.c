#include "./utils.h"

void sort_ast_type(AstType* ast_types, int begin, int end) {
    if (begin >= end) {
        return;
    }

    int i = begin;
    int j = end;
    AstType pivot = ast_types[begin];

    while (i < j) {
        while (i < j && ast_types[j] >= pivot) {
            j--;
        }
        ast_types[i] = ast_types[j];
        while (i < j && ast_types[i] <= pivot) {
            i++;
        }
        ast_types[j] = ast_types[i];
    }

    ast_types[i] = pivot;

    sort_ast_type(ast_types, begin, i - 1);
    sort_ast_type(ast_types, i + 1, end);
}

int ast_is_void_parameter_list(Ast* ast) {
    if (vector_size(ast->children) != 1) {
        return 0;
    }

    Ast* param_decl_ast = vector_at(ast->children, 0);

    Ast* param_decl_specs_ast = vector_at(param_decl_ast->children, 0);
    if (vector_size(param_decl_specs_ast->children) != 1) {
        return 0;
    }

    Ast* param_decl_spec_ast = vector_at(param_decl_specs_ast->children, 0);

    Ast* terminal_ast = vector_at(param_decl_ast->children, 1);

    return param_decl_spec_ast->type == AST_TYPE_VOID && terminal_ast->type == AST_ABS_DECLOR;
}
