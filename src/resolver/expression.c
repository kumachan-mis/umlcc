#include "./expression.h"
#include "../common/common.h"
#include "./conversion.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

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
        case AST_ADDR_EXPR:
        case AST_INDIR_EXPR:
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
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    Dtype* addr_dtype = new_pointer_dtype(dtype_copy(lhs_srt->dtype));
    Srt* addr_srt = new_dtyped_srt(SRT_ADDR_EXPR, addr_dtype, 1, lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    return new_dtyped_srt(SRT_ASSIGN_EXPR, dtype, 2, addr_srt, rhs_srt);
}

Srt* resolve_logical_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    switch (ast->type) {
        case AST_LOR_EXPR:
            resolver->ast = ast;
            return new_dtyped_srt(SRT_LOR_EXPR, dtype, 2, lhs_srt, rhs_srt);
        case AST_LAND_EXPR:
            return new_dtyped_srt(SRT_LAND_EXPR, dtype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_equality_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    switch (ast->type) {
        case AST_EQUAL_EXPR:
            return new_dtyped_srt(SRT_EQUAL_EXPR, dtype, 2, lhs_srt, rhs_srt);
        case AST_NEQUAL_EXPR:
            return new_dtyped_srt(SRT_NEQUAL_EXPR, dtype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_additive_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    resolver->ast = ast;

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {

        Dtype* dtype = new_integer_dtype();
        switch (ast->type) {
            case AST_ADD_EXPR:
                return new_dtyped_srt(SRT_ADD_EXPR, dtype, 2, lhs_srt, rhs_srt);
            case AST_SUB_EXPR:
                return new_dtyped_srt(SRT_SUB_EXPR, dtype, 2, lhs_srt, rhs_srt);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }

    } else if ((lhs_srt->dtype->type == DTYPE_POINTER && dtype_isarithmetic(rhs_srt->dtype)) ||
               (dtype_isarithmetic(lhs_srt->dtype) && rhs_srt->dtype->type == DTYPE_POINTER)) {

        if (rhs_srt->dtype->type == DTYPE_POINTER) swap_ptr(&lhs_srt, &rhs_srt);
        Dtype* dtype = dtype_copy(lhs_srt->dtype);
        switch (ast->type) {
            case AST_ADD_EXPR:
                return new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, lhs_srt, rhs_srt);
            case AST_SUB_EXPR:
                return new_dtyped_srt(SRT_PSUB_EXPR, dtype, 2, lhs_srt, rhs_srt);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }

    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {

        Dtype* dtype = new_integer_dtype();
        switch (ast->type) {
            case AST_SUB_EXPR:
                return new_dtyped_srt(SRT_PDIFF_EXPR, dtype, 2, rhs_srt, lhs_srt);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }
    }

    fprintf(stderr, "Error: unexpected operand, %d and %d\n", lhs_srt->dtype->type,
            rhs_srt->dtype->type);
    exit(1);
}

Srt* resolve_multiplicative_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    switch (ast->type) {
        case AST_MUL_EXPR:
            return new_dtyped_srt(SRT_MUL_EXPR, dtype, 2, lhs_srt, rhs_srt);
        case AST_DIV_EXPR:
            return new_dtyped_srt(SRT_DIV_EXPR, dtype, 2, lhs_srt, rhs_srt);
        case AST_MOD_EXPR:
            return new_dtyped_srt(SRT_MOD_EXPR, dtype, 2, lhs_srt, rhs_srt);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_unary_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* child_srt = resolve_expr(resolver);

    resolver->ast = ast;

    switch (ast->type) {
        case AST_ADDR_EXPR: {
            Dtype* dtype = new_pointer_dtype(dtype_copy(child_srt->dtype));
            return new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, child_srt);
        }
        case AST_INDIR_EXPR: {
            Dtype* dtype = dtype_copy(child_srt->dtype->pointer->to_dtype);
            return new_dtyped_srt(SRT_INDIR_EXPR, dtype, 1, child_srt);
        }
        case AST_LNOT_EXPR: {
            Dtype* dtype = new_integer_dtype();
            return new_dtyped_srt(SRT_LNOT_EXPR, dtype, 1, child_srt);
        }
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_postfix_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_CALL_EXPR: {
            resolver->ast = vector_at(ast->children, 0);
            Srt* lhs_srt = resolve_expr(resolver);

            // TODO: type conversion is always performed, not only in function calls
            lhs_srt = convert_function_to_ptr(lhs_srt);

            resolver->ast = vector_at(ast->children, 1);
            Srt* rhs_srt = resolve_argument_expr_list(resolver);

            Dtype* dtype = dtype_copy(lhs_srt->dtype->pointer->to_dtype->function->return_dtype);
            resolver->ast = ast;
            return new_dtyped_srt(SRT_CALL_EXPR, dtype, 2, lhs_srt, rhs_srt);
        }
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_argument_expr_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_ARG_LIST, 0);
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->ast = vector_at(ast->children, i);
        vector_push(srt->children, resolve_expr(resolver));
    }

    resolver->ast = ast;
    return srt;
}

Srt* resolve_primary_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;
    Symbol* symbol = NULL;

    switch (ast->type) {
        case AST_IDENT_EXPR:
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->local_table, ast->ident_name);
            }
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->global_table, ast->ident_name);
            }
            if (symbol == NULL) {
                fprintf(stderr, "Error: identifier '%s' is used before declared\n",
                        ast->ident_name);
                exit(1);
            }
            return new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(symbol->dtype),
                                      new_string(ast->ident_name));
        case AST_INT_EXPR:
            return new_integer_srt(SRT_INT_EXPR, ast->value_int);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}
