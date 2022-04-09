#include "./ast.h"

#include <stdlib.h>
#include <stdarg.h>


Ast* new_ast(AstType type, int num_children, ...) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->value_int = -1;
    ast->children = new_vector();

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(ast->children, va_arg(children, Ast*));
    }
    va_end(children);

    return ast;
}

Ast* new_integer_ast(int value) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = AST_INT;
    ast->value_int = value;
    ast->children = new_vector();
    return ast;
}

Ast* copy_ast(Ast* ast) {
    Ast* copied_ast = malloc(sizeof(Ast));
    copied_ast->type = ast->type;
    copied_ast->value_int = ast->value_int;
    copied_ast->children = new_vector();

    int num_children = vector_size(copied_ast->children);
    for (int i = 0; i < num_children; i++) {
        Ast* copied_child = copy_ast(vector_at(ast->children, i));
        vector_push(copied_ast->children, copied_child);
    }

    return copied_ast;
}

void delete_ast(Ast* ast) {
    delete_vector(ast->children, delete_ast);
    free(ast);
}
