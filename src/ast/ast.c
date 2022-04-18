#include "./ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


char* copy_ast_ident_name(char* ident_name);

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

Ast* new_identifier_expr_ast(char* name) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = AST_IDENT_EXPR;
    ast->ident_name = name;
    ast->value_int = -1;
    ast->children = new_vector();
    return ast;
}

Ast* new_identifier_declarator_ast(char* name) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = AST_IDENT_DECLOR;
    ast->ident_name = name;
    ast->value_int = -1;
    ast->children = new_vector();
    return ast;
}

Ast* new_integer_ast(int value) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = AST_INT_EXPR;
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

char* copy_ast_ident_name(char* ident_name) {
    if (ident_name == NULL) return NULL;
    char* copied_ident_name = malloc((strlen(ident_name) + 1) * sizeof(char));
    strcpy(copied_ident_name, ident_name);
    return copied_ident_name;
}

