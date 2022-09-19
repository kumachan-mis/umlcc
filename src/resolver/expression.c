#include "./expression.h"
#include "../common/type.h"
#include "../common/util.h"
#include "./conversion.h"

#include <stdlib.h>

ResolverReturn* resolve_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_ASSIGN_EXPR:
            resolverret_assign(&srt, &errs, resolve_assignment_expr(resolver));
            break;
        case AST_LOR_EXPR:
        case AST_LAND_EXPR:
            resolverret_assign(&srt, &errs, resolve_logical_expr(resolver));
            break;
        case AST_EQUAL_EXPR:
        case AST_NEQUAL_EXPR:
            resolverret_assign(&srt, &errs, resolve_equality_expr(resolver));
            break;
        case AST_ADD_EXPR:
        case AST_SUB_EXPR:
            resolverret_assign(&srt, &errs, resolve_additive_expr(resolver));
            break;
        case AST_MUL_EXPR:
        case AST_DIV_EXPR:
        case AST_MOD_EXPR:
            resolverret_assign(&srt, &errs, resolve_multiplicative_expr(resolver));
            break;
        case AST_ADDR_EXPR:
        case AST_INDIR_EXPR:
        case AST_LNOT_EXPR:
            resolverret_assign(&srt, &errs, resolve_unary_expr(resolver));
            break;
        case AST_SUBSC_EXPR:
        case AST_CALL_EXPR:
            resolverret_assign(&srt, &errs, resolve_postfix_expr(resolver));
            break;
        case AST_IDENT_EXPR:
        case AST_INT_EXPR:
        case AST_CHAR_EXPR:
        case AST_STRING_EXPR:
            resolverret_assign(&srt, &errs, resolve_primary_expr(resolver));
            break;
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverReturn* resolve_assignment_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    if (!srt_ismodifiable(lhs_srt)) {
        errs = new_vector(&t_error);
        err = new_error("Error: left operand of assignment should be modifiable lvalue\n");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_isassignable(lhs_srt->dtype, rhs_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("Error: expression is not assignable to lvalue\n");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_equals(rhs_srt->dtype, lhs_srt->dtype)) {
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(lhs_srt->dtype), 1, rhs_srt);
    }

    Dtype* dtype = dtype_copy(lhs_srt->dtype);
    lhs_srt = convert_to_ptr(lhs_srt);

    srt = new_dtyped_srt(SRT_ASSIGN_EXPR, dtype, 2, lhs_srt, rhs_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_logical_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    Dtype* dtype = new_integer_dtype(DTYPE_INT);

    switch (ast->type) {
        case AST_LOR_EXPR:
            if (!dtype_isscalar(lhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("Error: each of operands of || should have scalar type\n");
                vector_push(errs, err);
            }
            if (!dtype_isscalar(rhs_srt->dtype)) {
                if (errs == NULL) errs = new_vector(&t_error);
                err = new_error("Error: each of operands of || should have scalar type\n");
                vector_push(errs, err);
            }
            if (errs == NULL) srt = new_dtyped_srt(SRT_LOR_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_LAND_EXPR:
            if (!dtype_isscalar(lhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("Error: each of operands of && should have scalar type\n");
                vector_push(errs, err);
            }
            if (!dtype_isscalar(rhs_srt->dtype)) {
                if (errs == NULL) errs = new_vector(&t_error);
                err = new_error("Error: each of operands of && should have scalar type\n");
                vector_push(errs, err);
            }
            if (errs == NULL) srt = new_dtyped_srt(SRT_LAND_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_equality_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    Dtype* dtype = new_integer_dtype(DTYPE_INT);

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {
        lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);
        rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);
    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER &&
               dtype_iscompatible(lhs_srt->dtype, rhs_srt->dtype)) {
        // do nothing
    } else {
        errs = new_vector(&t_error);
    }

    switch (ast->type) {
        case AST_EQUAL_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("Error: operands of pointer == pointer are not compatible\n");
                } else {
                    err = new_error("Error: binary == expression should be "
                                    "either arithmetic == arithmetic or pointer == pointer\n");
                }
                vector_push(errs, err);
                break;
            }
            srt = new_dtyped_srt(SRT_EQUAL_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_NEQUAL_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("Error: operands of pointer != pointer are not compatible\n");
                } else {
                    err = new_error("Error: binary != expression should be "
                                    "either arithmetic != arithmetic or pointer != pointer\n");
                }
                vector_push(errs, err);
                break;
            }
            srt = new_dtyped_srt(SRT_NEQUAL_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            if (errs == NULL) errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_additive_expr(Resolver* resolver) {
    ResolverReturn* resolve_add_expr(Resolver * resolver);
    ResolverReturn* resolve_subtract_expr(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_ADD_EXPR:
            resolverret_assign(&srt, &errs, resolve_add_expr(resolver));
            break;
        case AST_SUB_EXPR:
            resolverret_assign(&srt, &errs, resolve_subtract_expr(resolver));
            break;
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverReturn* resolve_add_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {

        lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);
        rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);
        Dtype* dtype = new_integer_dtype(DTYPE_INT);
        srt = new_dtyped_srt(SRT_ADD_EXPR, dtype, 2, lhs_srt, rhs_srt);

    } else if ((lhs_srt->dtype->type == DTYPE_POINTER && dtype_isinteger(rhs_srt->dtype)) ||
               (dtype_isinteger(lhs_srt->dtype) && rhs_srt->dtype->type == DTYPE_POINTER)) {

        if (rhs_srt->dtype->type == DTYPE_POINTER) swap_ptr((void**)&lhs_srt, (void**)&rhs_srt);
        Dtype* dtype = dtype_copy(lhs_srt->dtype);
        srt = new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, lhs_srt, rhs_srt);

    } else {

        errs = new_vector(&t_error);
        err = new_error("Error: binary + expression should be either arithmetic + arithmetic, "
                        "pointer + integer, or integer + pointer\n");
        vector_push(errs, err);
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_subtract_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {

        lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);
        rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);
        Dtype* dtype = new_integer_dtype(DTYPE_INT);
        srt = new_dtyped_srt(SRT_SUB_EXPR, dtype, 2, lhs_srt, rhs_srt);

    } else if (lhs_srt->dtype->type == DTYPE_POINTER && dtype_isinteger(rhs_srt->dtype)) {

        Dtype* dtype = dtype_copy(lhs_srt->dtype);
        srt = new_dtyped_srt(SRT_PSUB_EXPR, dtype, 2, lhs_srt, rhs_srt);

    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER &&
               dtype_iscompatible(lhs_srt->dtype, rhs_srt->dtype)) {

        Dtype* dtype = new_integer_dtype(DTYPE_INT);
        srt = new_dtyped_srt(SRT_PDIFF_EXPR, dtype, 2, lhs_srt, rhs_srt);

    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {

        errs = new_vector(&t_error);
        err = new_error("Error: operands of pointer - pointer are not compatible\n");
        vector_push(errs, err);

    } else {

        errs = new_vector(&t_error);
        err = new_error("Error: binary - expression should be either arithmetic - arithmetic, "
                        "pointer - integer, or pointer - pointer\n");
        vector_push(errs, err);
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_multiplicative_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);
    lhs_srt = perform_usual_arithmetic_conversion(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);
    rhs_srt = perform_usual_arithmetic_conversion(rhs_srt);

    Dtype* dtype = new_integer_dtype(DTYPE_INT);

    switch (ast->type) {
        case AST_MUL_EXPR:
            if (!dtype_isarithmetic(lhs_srt->dtype) || !dtype_isarithmetic(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("Error: binary * expression should be arithmetic * arithmetic\n");
                vector_push(errs, err);
                break;
            }
            srt = new_dtyped_srt(SRT_MUL_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_DIV_EXPR:
            if (!dtype_isarithmetic(lhs_srt->dtype) || !dtype_isarithmetic(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("Error: binary / expression should be arithmetic / arithmetic\n");
                vector_push(errs, err);
                break;
            }
            srt = new_dtyped_srt(SRT_DIV_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_MOD_EXPR:
            if (!dtype_isinteger(lhs_srt->dtype) || !dtype_isinteger(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("Error: binary %% expression should be integer %% integer\n");
                vector_push(errs, err);
                break;
            }
            srt = new_dtyped_srt(SRT_MOD_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_unary_expr(Resolver* resolver) {
    ResolverReturn* resolve_address_expr(Resolver * resolver);
    ResolverReturn* resolve_indirection_expr(Resolver * resolver);
    ResolverReturn* resolve_logical_not_expr(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_ADDR_EXPR:
            resolverret_assign(&srt, &errs, resolve_address_expr(resolver));
            break;
        case AST_INDIR_EXPR:
            resolverret_assign(&srt, &errs, resolve_indirection_expr(resolver));
            break;
        case AST_LNOT_EXPR:
            resolverret_assign(&srt, &errs, resolve_logical_not_expr(resolver));
            break;
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverReturn* resolve_address_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    if (child_srt->dtype->type != DTYPE_FUNCTION && child_srt->type != SRT_INDIR_EXPR &&
        (child_srt->type != SRT_IDENT_EXPR || !dtype_isobject(child_srt->dtype))) {
        errs = new_vector(&t_error);
        err = new_error("Error: operand of unary & is neither a function designator, "
                        "a indirection, nor an object lvalue\n");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    Dtype* dtype = new_pointer_dtype(dtype_copy(child_srt->dtype));
    srt = new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_indirection_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;

    if (errs != NULL) return new_resolverret_errors(errs);

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);

    if (child_srt->dtype->type != DTYPE_POINTER) {
        errs = new_vector(&t_error);
        err = new_error("Error: operand of unary * does not have pointer type\n");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    Dtype* dtype = dtype_copy(child_srt->dtype->pointer->to_dtype);
    srt = new_dtyped_srt(SRT_INDIR_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_logical_not_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;

    if (errs != NULL) return new_resolverret_errors(errs);

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);

    if (!dtype_isscalar(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("Error: operand of unary ! does not have scalar type\n");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    srt = new_dtyped_srt(SRT_LNOT_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_postfix_expr(Resolver* resolver) {
    ResolverReturn* resolve_subscription_expr(Resolver * resolver);
    ResolverReturn* resolve_call_expr(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_SUBSC_EXPR:
            resolverret_assign(&srt, &errs, resolve_subscription_expr(resolver));
            break;
        case AST_CALL_EXPR:
            resolverret_assign(&srt, &errs, resolve_call_expr(resolver));
            break;
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverReturn* resolve_subscription_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    if ((lhs_srt->dtype->type != DTYPE_POINTER || !dtype_isinteger(rhs_srt->dtype)) &&
        (!dtype_isinteger(lhs_srt->dtype) || rhs_srt->dtype->type != DTYPE_POINTER)) {
        errs = new_vector(&t_error);
        err = new_error("Error: subscription should be pointer[integer] or integer[pointer]\n");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    if (rhs_srt->dtype->type == DTYPE_POINTER) swap_ptr((void**)&lhs_srt, (void**)&rhs_srt);

    if (!dtype_isobject(lhs_srt->dtype->pointer->to_dtype)) {
        errs = new_vector(&t_error);
        err = new_error("Error: subscribed object should have pointer to object type\n");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    Dtype* dtype = dtype_copy(lhs_srt->dtype);
    srt = new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, lhs_srt, rhs_srt);
    srt = new_dtyped_srt(SRT_INDIR_EXPR, dtype_copy(lhs_srt->dtype->pointer->to_dtype), 1, srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_call_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;

    if (errs != NULL) return new_resolverret_errors(errs);

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    if (lhs_srt->dtype->type != DTYPE_POINTER || lhs_srt->dtype->pointer->to_dtype->type != DTYPE_FUNCTION) {
        errs = new_vector(&t_error);
        Error* err = new_error("Error: called object is not a function or a function pointer\n");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    Dtype* call_dtype = resolver->call_dtype;
    resolver->call_dtype = lhs_srt->dtype->pointer->to_dtype;
    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_argument_expr_list(resolver));
    resolver->ast = ast;

    if (errs != NULL) {
        resolver->call_dtype = call_dtype;
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    Dtype* dtype = dtype_copy(resolver->call_dtype->function->return_dtype);
    srt = new_dtyped_srt(SRT_CALL_EXPR, dtype, 2, lhs_srt, rhs_srt);

    resolver->call_dtype = call_dtype;
    return new_resolverret(srt);
}

ResolverReturn* resolve_argument_expr_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_ARG_LIST, 0);
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;
    Dtype* call_dtype = resolver->call_dtype;

    int num_params = vector_size(call_dtype->function->params);
    int num_args = vector_size(ast->children);

    if (num_params != num_args) {
        errs = new_vector(&t_error);
        err = new_error("Error: function takes %d params, but passed %d arguments\n", num_params, num_args);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    for (int i = 0; i < num_args; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;
        DParam* dparam = vector_at(call_dtype->function->params, i);

        resolver->ast = vector_at(ast->children, i);
        resolverret_assign(&child_srt, &child_errs, resolve_expr(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) errs = new_vector(&t_error);
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        }

        child_srt = convert_to_ptr_if_array(child_srt);
        child_srt = convert_to_ptr_if_function(child_srt);

        if (!dtype_isassignable(dparam->dtype, child_srt->dtype)) {
            if (errs == NULL) errs = new_vector(&t_error);
            err = new_error("Error: argument is not assignable to parameter\n");
            vector_push(errs, err);
        }
        if (errs != NULL) {
            delete_srt(child_srt);
            continue;
        }

        if (!dtype_equals(dparam->dtype, child_srt->dtype)) {
            child_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dparam->dtype), 1, child_srt);
        }

        vector_push(srt->children, child_srt);
    }

    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_primary_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_IDENT_EXPR: {
            Symbol* symbol = NULL;
            if (symbol == NULL && resolver->local_table != NULL) {
                symbol = symboltable_search(resolver->local_table, ast->ident_name);
            }
            if (symbol == NULL) symbol = symboltable_search(resolver->global_table, ast->ident_name);
            if (symbol == NULL) {
                errs = new_vector(&t_error);
                err = new_error("Error: identifier '%s' is used before declared\n", ast->ident_name);
                vector_push(errs, err);
                break;
            }
            Dtype* ident_dtype = dtype_copy(symbol->dtype);
            char* ident_name = new_string(symbol->name);
            srt = new_identifier_srt(SRT_IDENT_EXPR, ident_dtype, ident_name);
            break;
        }
        case AST_INT_EXPR: {
            Dtype* dtype = new_integer_dtype(DTYPE_INT);
            IntegerLiteral* iliteral = iliteral_copy(ast->iliteral);
            srt = new_iliteral_srt(SRT_INT_EXPR, dtype, iliteral);
            break;
        }
        case AST_CHAR_EXPR: {
            Dtype* dtype = new_integer_dtype(DTYPE_INT);
            IntegerLiteral* iliteral = iliteral_copy(ast->iliteral);
            srt = new_iliteral_srt(SRT_CHAR_EXPR, dtype, iliteral);
            break;
        }
        case AST_STRING_EXPR: {
            resolver->sliteral_id++;
            char* sliteral_label = sliteral_create_label(resolver->sliteral_id);
            Dtype* decl_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), ast->sliteral->size);
            Srt* decl_srt = new_identifier_srt(SRT_DECL, decl_dtype, sliteral_label);

            Dtype* init_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), ast->sliteral->size);
            StringLiteral* sliteral = sliteral_copy(ast->sliteral);
            Srt* literal_srt = new_sliteral_srt(SRT_STRING_EXPR, init_dtype, sliteral);
            Srt* init_srt = new_srt(SRT_INIT, 1, literal_srt);

            Srt* decl_list_srt = new_srt(SRT_DECL_LIST, 1, new_srt(SRT_INIT_DECL, 2, decl_srt, init_srt));
            vector_push(resolver->trans_unit_srt->children, decl_list_srt);

            Dtype* ident_dtype = dtype_copy(decl_dtype);
            char* ident_name = new_string(sliteral_label);
            srt = new_identifier_srt(SRT_IDENT_EXPR, ident_dtype, ident_name);
            break;
        }
        default:
            errs = new_vector(&t_error);
            err = new_error("Error: unreachable statement (ast_type=%s)\n", ast_types[ast->type]);
            vector_push(errs, err);
            break;
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}
