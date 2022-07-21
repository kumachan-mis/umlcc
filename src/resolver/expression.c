#include "./expression.h"
#include "../common/type.h"
#include "../common/util.h"
#include "./conversion.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        case AST_CHAR_EXPR:
        case AST_STRING_EXPR:
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
    lhs_srt = convert_to_ptr(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    if (!dtype_equals(rhs_srt->dtype, lhs_srt->dtype->pointer->to_dtype)) {
        Dtype* dtype = dtype_copy(lhs_srt->dtype->pointer->to_dtype);
        lhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype, 1, lhs_srt);
    }

    Dtype* dtype = dtype_copy(lhs_srt->dtype->pointer->to_dtype);
    resolver->ast = ast;

    return new_dtyped_srt(SRT_ASSIGN_EXPR, dtype, 2, lhs_srt, rhs_srt);
}

Srt* resolve_logical_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* lhs_srt = resolve_expr(resolver);
    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    Dtype* dtype = new_integer_dtype(DTYPE_INT);
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
    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    resolver->ast = ast;

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {
        lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);
        rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);
    }

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
    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    resolver->ast = ast;

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {
        lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);
        rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);
        Dtype* dtype = new_integer_dtype(DTYPE_INT);
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

        if (rhs_srt->dtype->type == DTYPE_POINTER) swap_ptr((void**)&lhs_srt, (void**)&rhs_srt);
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

        Dtype* dtype = new_integer_dtype(DTYPE_INT);
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
    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);
    lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    Srt* rhs_srt = resolve_expr(resolver);
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);
    rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);

    Dtype* dtype = new_integer_dtype(DTYPE_INT);
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
            child_srt = convert_to_ptr_if_array(child_srt);
            child_srt = convert_to_ptr_if_function(child_srt);
            Dtype* dtype = dtype_copy(child_srt->dtype->pointer->to_dtype);
            return new_dtyped_srt(SRT_INDIR_EXPR, dtype, 1, child_srt);
        }
        case AST_LNOT_EXPR: {
            child_srt = convert_to_ptr_if_array(child_srt);
            child_srt = convert_to_ptr_if_function(child_srt);
            Dtype* dtype = new_integer_dtype(DTYPE_INT);
            return new_dtyped_srt(SRT_LNOT_EXPR, dtype, 1, child_srt);
        }
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}

Srt* resolve_postfix_expr(Resolver* resolver) {
    Ast* ast = resolver->ast;
    Dtype* function_dtype = resolver->function_dtype;

    switch (ast->type) {
        case AST_SUBSC_EXPR: {
            Ast* lhs_ast = ast_copy(vector_at(ast->children, 0));
            Ast* rhs_ast = ast_copy(vector_at(ast->children, 1));
            resolver->ast = new_ast(AST_INDIR_EXPR, 1, new_ast(AST_ADD_EXPR, 2, lhs_ast, rhs_ast));
            Srt* srt = resolve_expr(resolver);
            delete_ast(resolver->ast);
            resolver->ast = ast;
            return srt;
        }
        case AST_CALL_EXPR: {
            resolver->ast = vector_at(ast->children, 0);
            Srt* lhs_srt = resolve_expr(resolver);
            lhs_srt = convert_to_ptr_if_array(lhs_srt);
            lhs_srt = convert_to_ptr_if_function(lhs_srt);

            resolver->ast = vector_at(ast->children, 1);
            resolver->function_dtype = lhs_srt->dtype->pointer->to_dtype;
            Srt* rhs_srt = resolve_argument_expr_list(resolver);

            Dtype* dtype = dtype_copy(resolver->function_dtype->function->return_dtype);
            resolver->function_dtype = function_dtype;
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
    Dtype* function_dtype = resolver->function_dtype;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        DParam* dparam = vector_at(function_dtype->function->params, i);

        resolver->ast = vector_at(ast->children, i);
        Srt* child_srt = resolve_expr(resolver);
        child_srt = convert_to_ptr_if_array(child_srt);
        child_srt = convert_to_ptr_if_function(child_srt);

        if (!dtype_equals(dparam->dtype, child_srt->dtype)) {
            child_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dparam->dtype), 1, child_srt);
        }

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
            if (symbol == NULL && resolver->local_table != NULL) {
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
            return new_integer_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                   ast->iliteral->signed_value);
        case AST_CHAR_EXPR:
            return new_integer_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                   ast->iliteral->signed_value);
        case AST_STRING_EXPR: {
            int size = ast->sliteral->size;

            resolver->sliteral_id++;
            char* literal_name = create_sliteral_name(resolver->sliteral_id);
            Dtype* decl_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), size);
            Srt* decl_srt = new_identifier_srt(SRT_DECL, decl_dtype, literal_name);

            Dtype* init_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), size);
            StringLiteral* sliteral = sliteral_copy(ast->sliteral);
            Srt* literal_srt = new_sliteral_srt(SRT_STRING_EXPR, init_dtype, sliteral);
            Srt* init_srt = new_srt(SRT_INIT, 1, literal_srt);

            Srt* srt = new_srt(SRT_DECL_LIST, 1, new_srt(SRT_INIT_DECL, 2, decl_srt, init_srt));
            vector_push(resolver->trans_unit_srt->children, srt);

            Dtype* ident_dtype = dtype_copy(decl_dtype);
            char* ident_name = new_string(literal_name);
            return new_identifier_srt(SRT_IDENT_EXPR, ident_dtype, ident_name);
        }
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
            exit(1);
    }
}
