#include "./expression.h"
#include "../common/type.h"
#include "../common/util.h"
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
        case AST_SUBSC_EXPR:
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
    Srt* lhs = resolve_expr(resolver);
    lhs = convert_to_ptr(lhs);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);
    rhs = convert_to_ptr_if_array(rhs);
    rhs = convert_to_ptr_if_function(rhs);

    Dtype* dtype = dtype_copy(lhs->dtype->pointer->to_dtype);
    resolver->ast = ast;

    return new_dtyped_srt(SRT_ASSIGN_EXPR, dtype, 2, lhs, rhs);
}

Srt* resolve_logical_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs = resolve_expr(resolver);
    lhs = convert_to_ptr_if_array(lhs);
    lhs = convert_to_ptr_if_function(lhs);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);
    rhs = convert_to_ptr_if_array(rhs);
    rhs = convert_to_ptr_if_function(rhs);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    switch (ast->type) {
        case AST_LOR_EXPR:
            resolver->ast = ast;
            return new_dtyped_srt(SRT_LOR_EXPR, dtype, 2, lhs, rhs);
        case AST_LAND_EXPR:
            return new_dtyped_srt(SRT_LAND_EXPR, dtype, 2, lhs, rhs);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_equality_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs = resolve_expr(resolver);
    lhs = convert_to_ptr_if_array(lhs);
    lhs = convert_to_ptr_if_function(lhs);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);
    rhs = convert_to_ptr_if_array(rhs);
    rhs = convert_to_ptr_if_function(rhs);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    switch (ast->type) {
        case AST_EQUAL_EXPR:
            return new_dtyped_srt(SRT_EQUAL_EXPR, dtype, 2, lhs, rhs);
        case AST_NEQUAL_EXPR:
            return new_dtyped_srt(SRT_NEQUAL_EXPR, dtype, 2, lhs, rhs);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_additive_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs = resolve_expr(resolver);
    lhs = convert_to_ptr_if_array(lhs);
    lhs = convert_to_ptr_if_function(lhs);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);
    rhs = convert_to_ptr_if_array(rhs);
    rhs = convert_to_ptr_if_function(rhs);

    resolver->ast = ast;

    if (dtype_isarithmetic(lhs->dtype) && dtype_isarithmetic(rhs->dtype)) {

        Dtype* dtype = new_integer_dtype();
        switch (ast->type) {
            case AST_ADD_EXPR:
                return new_dtyped_srt(SRT_ADD_EXPR, dtype, 2, lhs, rhs);
            case AST_SUB_EXPR:
                return new_dtyped_srt(SRT_SUB_EXPR, dtype, 2, lhs, rhs);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }

    } else if ((lhs->dtype->type == DTYPE_POINTER && dtype_isarithmetic(rhs->dtype)) ||
               (dtype_isarithmetic(lhs->dtype) && rhs->dtype->type == DTYPE_POINTER)) {

        if (rhs->dtype->type == DTYPE_POINTER) swap_ptr((void**)&lhs, (void**)&rhs);
        Dtype* dtype = dtype_copy(lhs->dtype);
        switch (ast->type) {
            case AST_ADD_EXPR:
                return new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, lhs, rhs);
            case AST_SUB_EXPR:
                return new_dtyped_srt(SRT_PSUB_EXPR, dtype, 2, lhs, rhs);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }

    } else if (lhs->dtype->type == DTYPE_POINTER && rhs->dtype->type == DTYPE_POINTER) {

        Dtype* dtype = new_integer_dtype();
        switch (ast->type) {
            case AST_SUB_EXPR:
                return new_dtyped_srt(SRT_PDIFF_EXPR, dtype, 2, rhs, lhs);
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        }
    }

    fprintf(stderr, "Error: unexpected operand, %d and %d\n", lhs->dtype->type, rhs->dtype->type);
    exit(1);
}

Srt* resolve_multiplicative_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs = resolve_expr(resolver);
    lhs = convert_to_ptr_if_array(lhs);
    lhs = convert_to_ptr_if_function(lhs);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs = resolve_expr(resolver);
    rhs = convert_to_ptr_if_array(rhs);
    rhs = convert_to_ptr_if_function(rhs);

    Dtype* dtype = new_integer_dtype();
    resolver->ast = ast;

    switch (ast->type) {
        case AST_MUL_EXPR:
            return new_dtyped_srt(SRT_MUL_EXPR, dtype, 2, lhs, rhs);
        case AST_DIV_EXPR:
            return new_dtyped_srt(SRT_DIV_EXPR, dtype, 2, lhs, rhs);
        case AST_MOD_EXPR:
            return new_dtyped_srt(SRT_MOD_EXPR, dtype, 2, lhs, rhs);
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
            child_srt = convert_to_ptr_if_array(child_srt);
            child_srt = convert_to_ptr_if_function(child_srt);
            Dtype* dtype = dtype_copy(child_srt->dtype->pointer->to_dtype);
            return new_dtyped_srt(SRT_INDIR_EXPR, dtype, 1, child_srt);
        }
        case AST_LNOT_EXPR: {
            child_srt = convert_to_ptr_if_array(child_srt);
            child_srt = convert_to_ptr_if_function(child_srt);
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
        case AST_SUBSC_EXPR: {
            Ast* lhs = ast_copy(vector_at(ast->children, 0));
            Ast* rhs = ast_copy(vector_at(ast->children, 1));
            resolver->ast = new_ast(AST_INDIR_EXPR, 1, new_ast(AST_ADD_EXPR, 2, lhs, rhs));
            Srt* srt = resolve_expr(resolver);
            delete_ast(resolver->ast);
            resolver->ast = ast;
            return srt;
        }
        case AST_CALL_EXPR: {
            resolver->ast = vector_at(ast->children, 0);
            Srt* lhs = resolve_expr(resolver);
            lhs = convert_to_ptr_if_array(lhs);
            lhs = convert_to_ptr_if_function(lhs);

            resolver->ast = vector_at(ast->children, 1);
            Srt* rhs = resolve_argument_expr_list(resolver);

            Dtype* dtype = dtype_copy(lhs->dtype->pointer->to_dtype->function->return_dtype);
            resolver->ast = ast;
            return new_dtyped_srt(SRT_CALL_EXPR, dtype, 2, lhs, rhs);
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
        Srt* child_srt = resolve_expr(resolver);
        child_srt = convert_to_ptr_if_array(child_srt);
        child_srt = convert_to_ptr_if_function(child_srt);
        vector_push(srt->children, child_srt);
    }

    resolver->ast = ast;
    return srt;
}

Srt* resolve_primary_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_IDENT_EXPR: {
            Symbol* symbol = NULL;
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->local_table, ast->ident_name);
            }
            if (symbol == NULL) {
                symbol = symboltable_search(resolver->global_table, ast->ident_name);
            }
            Dtype* ident_dtype = dtype_copy(symbol->dtype);
            char* ident_name = new_string(symbol->name);
            return new_identifier_srt(SRT_IDENT_EXPR, ident_dtype, ident_name);
        }
        case AST_INT_EXPR:
            return new_integer_srt(SRT_INT_EXPR, ast->value_int);
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}
