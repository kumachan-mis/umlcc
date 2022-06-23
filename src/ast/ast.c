#include "./ast.h"
#include "../common/util.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

BaseType t_ast = {
    .copy_object = (void* (*)(void*))ast_copy,
    .delete_object = (void (*)(void*))delete_ast,
};

Ast* new_ast(AstType type, int num_children, ...) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->value_int = -1;
    ast->value_str = NULL;
    ast->size_str = -1;
    ast->children = new_vector(&t_ast);

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
    ast->value_str = NULL;
    ast->size_str = -1;
    ast->children = new_vector(&t_ast);
    return ast;
}

Ast* new_integer_ast(AstType type, int value) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->value_int = value;
    ast->value_str = NULL;
    ast->size_str = -1;
    ast->children = new_vector(&t_ast);
    return ast;
}

Ast* new_string_literal_ast(AstType type, char* value, int size) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->value_int = -1;
    ast->value_str = value;
    ast->size_str = size;
    ast->children = new_vector(&t_ast);
    return ast;
}

Ast* ast_copy(Ast* ast) {
    Ast* copied_ast = malloc(sizeof(Ast));
    copied_ast->type = ast->type;
    copied_ast->ident_name = NULL;
    if (ast->ident_name != NULL) copied_ast->ident_name = new_string(ast->ident_name);
    copied_ast->value_int = ast->value_int;
    copied_ast->value_str = NULL;
    if (ast->value_str != NULL) copied_ast->value_str = copy_charmem(ast->value_str, ast->size_str);
    copied_ast->size_str = ast->size_str;
    copied_ast->children = vector_copy(ast->children);
    return copied_ast;
}

void delete_ast(Ast* ast) {
    if (ast->ident_name != NULL) free(ast->ident_name);
    if (ast->value_str != NULL) free(ast->value_str);
    delete_vector(ast->children);
    free(ast);
}
