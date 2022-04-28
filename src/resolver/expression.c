#include "./expression.h"
#include "./conversion.h"
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
    Srt* lhs_srt = new_srt(SRT_ADDR_EXPR, 1, resolve_expr(resolver));

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();
    resolver->_ast = ast;

    return new_ctyped_srt(SRT_ASSIGN_EXPR, ctype, 2, lhs_srt, rhs_srt);
}

Srt* resolve_additive_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    CType* ctype =  new_integer_ctype();
    resolver->_ast = ast;

    switch (ast->type) {
        case AST_ADD_EXPR:
            resolver->_ast = ast;
            return new_ctyped_srt(SRT_ADD_EXPR, ctype, 2, lhs_srt, rhs_srt);
        case AST_SUB_EXPR:
            return new_ctyped_srt(SRT_SUB_EXPR, ctype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_multiplicative_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();
    resolver->_ast = ast;

    switch (ast->type) {
        case AST_MUL_EXPR:
            return new_ctyped_srt(SRT_MUL_EXPR, ctype, 2, lhs_srt, rhs_srt);
        case AST_DIV_EXPR:
            return new_ctyped_srt(SRT_DIV_EXPR, ctype, 2, lhs_srt, rhs_srt);
        case AST_MOD_EXPR:
            return new_ctyped_srt(SRT_MOD_EXPR, ctype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_postfix_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    switch (ast->type) {
        case AST_CALL_EXPR: {
            resolver->_ast = vector_at(ast->children, 0);
            Srt* lhs_srt = convert_function_to_ptr(resolve_expr(resolver));
            // TODO: type conversion is always performed, not only in function calls
        
            resolver->_ast = vector_at(ast->children, 1);
            Srt* rhs_srt = resolve_argument_expr_list(resolver);
            
            CType* ctype = ctype_copy(ctype->function->return_type);
            resolver->_ast = ast;
            return new_ctyped_srt(SRT_CALL_EXPR, ctype, 2, lhs_srt, rhs_srt);
        }
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_argument_expr_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_ARG_LIST, 0);
    Ast* ast = resolver->_ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        vector_push(srt->children, resolve_expr(resolver));
    }

    resolver->_ast = ast;
    return srt;
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
