#include "./ast.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

BaseType t_ast = {
    .copy_object = (void* (*)(void*))ast_copy,
    .delete_object = (void (*)(void*))delete_ast,
};

char* ast_types[] = {
    // external definitions
    "translation-unit", "function-definition",
    // declarations
    "declaration", "declaration-specifiers", "init-declarator-list", "init-declarator", "typedef", "char", "int",
    "typedef-name", "identifier-declarator", "pointer-declarator", "array-declarator", "function-declarator",
    "parameter-list", "parameter-declaration", "initializer-list",
    // statements
    "compound-statement", "return-statement", "expression-statement",
    // expressions
    "assign-expression", "logical-OR-expression", "logical-AND-expression", "equal-expression", "not-equal-expression",
    "add-expression", "subtract-expression", "multiply-expression", "divide-expression", "modulo-expression",
    "address-expression", "indirection-expression", "logical-NOT-expression", "subscribe-expression", "call-expression",
    "argument-list", "identifier", "integer-constant", "character-constant", "string-literal"};

Ast* new_ast(AstType type, int num_children, ...) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->iliteral = NULL;
    ast->sliteral = NULL;
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
    ast->iliteral = NULL;
    ast->sliteral = NULL;
    ast->children = new_vector(&t_ast);
    return ast;
}

Ast* new_iliteral_ast(AstType type, IntegerLiteral* iliteral) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->iliteral = iliteral;
    ast->sliteral = NULL;
    ast->children = new_vector(&t_ast);
    return ast;
}

Ast* new_sliteral_ast(AstType type, StringLiteral* sliteral) {
    Ast* ast = malloc(sizeof(Ast));
    ast->type = type;
    ast->ident_name = NULL;
    ast->iliteral = NULL;
    ast->sliteral = sliteral;
    ast->children = new_vector(&t_ast);
    return ast;
}

Ast* ast_copy(Ast* ast) {
    Ast* copied_ast = malloc(sizeof(Ast));
    copied_ast->type = ast->type;
    copied_ast->ident_name = NULL;
    if (ast->ident_name != NULL) copied_ast->ident_name = new_string(ast->ident_name);
    copied_ast->iliteral = NULL;
    if (ast->iliteral != NULL) copied_ast->iliteral = iliteral_copy(ast->iliteral);
    copied_ast->sliteral = NULL;
    if (ast->sliteral != NULL) copied_ast->sliteral = sliteral_copy(ast->sliteral);
    copied_ast->children = vector_copy(ast->children);
    return copied_ast;
}

int ast_isexpr(Ast* ast) {
    return AST_ASSIGN_EXPR <= ast->type && ast->type <= AST_STRING_EXPR;
}

void delete_ast(Ast* ast) {
    if (ast->ident_name != NULL) free(ast->ident_name);
    if (ast->iliteral != NULL) delete_iliteral(ast->iliteral);
    if (ast->sliteral != NULL) delete_sliteral(ast->sliteral);
    delete_vector(ast->children);
    free(ast);
}
