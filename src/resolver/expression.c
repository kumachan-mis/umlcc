#include "./expression.h"
#include "../common/common.h"
#include "./conversion.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    switch (ast->type) {
        case AST_ASSIGN_EXPR:
            return resolve_assignment_expr(resolver);
        case AST_LOR_EXPR:
        case AST_LAND_EXPR:
            return resolve_logical_expr(resolver);
        case AST_EQUAL_EXPR:
        case AST_NEQUAL_EXPR:
            return resolve_equality_expr(resolver);
        case AST_ADD_EXPR:
        case AST_SUB_EXPR:
            return resolve_additive_expr(resolver);
        case AST_MUL_EXPR:
        case AST_DIV_EXPR:
        case AST_MOD_EXPR:
            return resolve_multiplicative_expr(resolver);
        case AST_LNOT_EXPR:
            return resolve_unary_expr(resolver);
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
    Srt* lhs_srt = resolve_expr(resolver);

    CType* addr_ctype = new_pointer_ctype(ctype_copy(lhs_srt->ctype));
    Srt* addr_srt = new_ctyped_srt(SRT_ADDR_EXPR, addr_ctype, 1, lhs_srt);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();
    resolver->_ast = ast;

    return new_ctyped_srt(SRT_ASSIGN_EXPR, ctype, 2, addr_srt, rhs_srt);
}

Srt* resolve_logical_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();
    resolver->_ast = ast;

    switch (ast->type) {
        case AST_LOR_EXPR:
            resolver->_ast = ast;
            return new_ctyped_srt(SRT_LOR_EXPR, ctype, 2, lhs_srt, rhs_srt);
        case AST_LAND_EXPR:
            return new_ctyped_srt(SRT_LAND_EXPR, ctype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_equality_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    CType* ctype = new_integer_ctype();
    resolver->_ast = ast;

    switch (ast->type) {
        case AST_EQUAL_EXPR:
            return new_ctyped_srt(SRT_EQUAL_EXPR, ctype, 2, lhs_srt, rhs_srt);
        case AST_NEQUAL_EXPR:
            return new_ctyped_srt(SRT_NEQUAL_EXPR, ctype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_additive_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    resolver->_ast = ast;

    if (ctype_isarithmetic(lhs_srt->ctype) && ctype_isarithmetic(rhs_srt->ctype)) {

        CType* ctype = new_integer_ctype();
        switch (ast->type) {
            case AST_ADD_EXPR:
                return new_ctyped_srt(SRT_ADD_EXPR, ctype, 2, lhs_srt, rhs_srt);
            case AST_SUB_EXPR:
                return new_ctyped_srt(SRT_SUB_EXPR, ctype, 2, lhs_srt, rhs_srt);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }

    } else if ((lhs_srt->ctype->type == CTYPE_POINTER && ctype_isarithmetic(rhs_srt->ctype)) ||
               (ctype_isarithmetic(lhs_srt->ctype) && rhs_srt->ctype->type == CTYPE_POINTER)) {

        if (rhs_srt->ctype->type == CTYPE_POINTER) swap_ptr(&lhs_srt, &rhs_srt);
        CType* ctype = ctype_copy(lhs_srt->ctype);
        switch (ast->type) {
            case AST_ADD_EXPR:
                return new_ctyped_srt(SRT_PADD_EXPR, ctype, 2, lhs_srt, rhs_srt);
            case AST_SUB_EXPR:
                return new_ctyped_srt(SRT_PSUB_EXPR, ctype, 2, lhs_srt, rhs_srt);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }

    } else if (lhs_srt->ctype->type == CTYPE_POINTER && rhs_srt->ctype->type == CTYPE_POINTER) {

        CType* ctype = new_integer_ctype();
        switch (ast->type) {
            case AST_SUB_EXPR:
                return new_ctyped_srt(SRT_PDIFF_EXPR, ctype, 2, rhs_srt, lhs_srt);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }
    }

    fprintf(stderr, "Error: unexpected operand, %d and %d\n", lhs_srt->ctype->type,
            rhs_srt->ctype->type);
    exit(1);
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

Srt* resolve_unary_expr(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* child_srt = resolve_expr(resolver);

    resolver->_ast = ast;

    switch (ast->type) {
        case AST_ADDR_EXPR: {
            CType* ctype = new_pointer_ctype(ctype_copy(child_srt->ctype));
            return new_ctyped_srt(SRT_ADDR_EXPR, ctype, 1, child_srt);
        }
        case AST_INDIR_EXPR: {
            CType* ctype = ctype_copy(child_srt->ctype->pointer->to_ctype);
            return new_ctyped_srt(SRT_INDIR_EXPR, ctype, 1, child_srt);
        }
        case AST_LNOT_EXPR: {
            CType* ctype = new_integer_ctype();
            return new_ctyped_srt(SRT_LNOT_EXPR, ctype, 1, child_srt);
        }
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
            Srt* raw_lhs_srt = resolve_expr(resolver);
            Srt* lhs_srt = convert_function_to_ptr(raw_lhs_srt);
            // TODO: type conversion is always performed, not only in function calls

            resolver->_ast = vector_at(ast->children, 1);
            Srt* rhs_srt = resolve_argument_expr_list(resolver);

            CType* ctype = ctype_copy(raw_lhs_srt->ctype->function->return_ctype);
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
    Symbol* symbol = NULL;

    switch (ast->type) {
        case AST_IDENT_EXPR:
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->_local_table, ast->ident_name);
            }
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->_global_table, ast->ident_name);
            }
            if (symbol == NULL) {
                fprintf(stderr, "Error: identifier '%s' is used before declared\n",
                        ast->ident_name);
                exit(1);
            }
            return new_identifier_srt(SRT_IDENT_EXPR, ctype_copy(symbol->ctype),
                                      new_string(ast->ident_name));
        case AST_INT_EXPR:
            return new_integer_srt(SRT_INT_EXPR, ast->value_int);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}
