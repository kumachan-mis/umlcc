#include "./expression.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>


Srt* resolve_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;
    switch (ast->type) {
        case AST_ASSIGN_EXPR:
            return resolve_assignment_expr(resolver);
        case AST_ADD_EXPR:
        case AST_SUB_EXPR:
            return resolve_additive_expr(resolver);
        case AST_MUL_EXPR:
        case AST_DIV_EXPR:
        case AST_MOD_EXPR:
            return resolve_multiplicative_expr(resolver);
        case AST_CALL_EXPR:
            return resolve_postfix_expr(resolver);
        case AST_IDENT_EXPR:
        case AST_INT_EXPR:
            return resolve_primary_expr(resolver);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);

    }
}

Srt* resolve_assignment_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs = new_srt(SRT_ADDR_EXPR, 1, resolve_expr(resolver));

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();
    return new_ctyped_srt(SRT_ASSIGN_EXPR, ctype, 2, lhs, rhs);
}

Srt* resolve_additive_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);

    CType* ctype =  new_integer_ctype();

    switch (ast->type) {
        case AST_ADD_EXPR:
            return new_ctyped_srt(SRT_ADD_EXPR, ctype, 2, lhs, rhs);
        case AST_SUB_EXPR:
            return new_ctyped_srt(SRT_SUB_EXPR, ctype, 2, lhs, rhs);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_multiplicative_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();

    switch (ast->type) {
        case AST_MUL_EXPR:
            return new_ctyped_srt(SRT_MUL_EXPR, ctype, 2, lhs, rhs);
        case AST_DIV_EXPR:
            return new_ctyped_srt(SRT_DIV_EXPR, ctype, 2, lhs, rhs);
        case AST_MOD_EXPR:
            return new_ctyped_srt(SRT_MOD_EXPR, ctype, 2, lhs, rhs);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_postfix_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* child = resolve_expr(resolver);

    CType* ctype = ctype_copy(child->ctype);

    switch (ast->type) {
        case AST_CALL_EXPR:
            child = new_ctyped_srt(SRT_ADDR_EXPR, new_pointer_ctype(ctype), 1, child);
            // function -> pointer to function. TODO: implicit conversion as a module.
            return new_ctyped_srt(SRT_CALL_EXPR, ctype_copy(ctype->function->return_type), 1, child);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_primary_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    switch (ast->type) {
        case AST_IDENT_EXPR: {
            Symbol* symbol = NULL;
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->_local_table, ast->ident_name);
            }
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->_global_table, ast->ident_name);
            }
            if (symbol == NULL) {
                fprintf(stderr, "Error: identifier '%s' is used before declared\n", ast->ident_name);
                exit(1);
            }
            return new_identifier_srt(SRT_IDENT_EXPR, ctype_copy(symbol->ctype), string_copy(ast->ident_name));
        }
        case AST_INT_EXPR:
            return new_integer_srt(SRT_INT_EXPR, ast->value_int);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}
