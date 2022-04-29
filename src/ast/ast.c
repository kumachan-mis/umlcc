#include "./ast.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

Ast* new_ast(AstType type, int num_children, ...) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
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

Ast* new_identifier_ast(AstType type, char* name) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = name;
    ast->value_int = -1;
    ast->children = new_vector();
    return ast;
}

Ast* new_integer_ast(AstType type, int value) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->value_int = value;
    ast->children = new_vector();
    return ast;
}

void delete_ast(Ast* ast) {
    if (ast->ident_name != NULL) free(ast->ident_name);
    delete_vector(ast->children, (void (*)(void* item))delete_ast);
    free(ast);
}
