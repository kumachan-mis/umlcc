#include "./expression.h"
#include "../common/type.h"
#include "./conversion.h"
#include "./declaration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ResolverReturn* resolve_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_ASSIGN_EXPR:
            resolverret_assign(&srt, &errs, resolve_assignment_expr(resolver));
            break;
        case AST_MUL_ASSIGN_EXPR:
        case AST_DIV_ASSIGN_EXPR:
        case AST_MOD_ASSIGN_EXPR:
        case AST_ADD_ASSIGN_EXPR:
        case AST_SUB_ASSIGN_EXPR:
        case AST_OR_ASSIGN_EXPR:
        case AST_XOR_ASSIGN_EXPR:
        case AST_AND_ASSIGN_EXPR:
            resolverret_assign(&srt, &errs, resolve_compound_assignment_expr(resolver));
            break;
        case AST_COND_EXPR:
            resolverret_assign(&srt, &errs, resolve_conditional_expr(resolver));
            break;
        case AST_LOR_EXPR:
        case AST_LAND_EXPR:
            resolverret_assign(&srt, &errs, resolve_logical_expr(resolver));
            break;
        case AST_OR_EXPR:
        case AST_XOR_EXPR:
        case AST_AND_EXPR:
            resolverret_assign(&srt, &errs, resolve_bitwise_expr(resolver));
            break;
        case AST_EQUAL_EXPR:
        case AST_NEQUAL_EXPR:
            resolverret_assign(&srt, &errs, resolve_equality_expr(resolver));
            break;
        case AST_LESS_EXPR:
        case AST_GREATER_EXPR:
        case AST_LESSEQ_EXPR:
        case AST_GREATEREQ_EXPR:
            resolverret_assign(&srt, &errs, resolve_relational_expr(resolver));
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
        case AST_CAST_EXPR:
            resolverret_assign(&srt, &errs, resolve_cast_expr(resolver));
            break;
        case AST_PREINC_EXPR:
        case AST_PREDEC_EXPR:
        case AST_ADDR_EXPR:
        case AST_INDIR_EXPR:
        case AST_POS_EXPR:
        case AST_NEG_EXPR:
        case AST_NOT_EXPR:
        case AST_LNOT_EXPR:
        case AST_SIZEOF_EXPR:
            resolverret_assign(&srt, &errs, resolve_unary_expr(resolver));
            break;
        case AST_SUBSC_EXPR:
        case AST_CALL_EXPR:
        case AST_MEMBER_EXPR:
        case AST_TOMEMBER_EXPR:
        case AST_POSTINC_EXPR:
        case AST_POSTDEC_EXPR:
            resolverret_assign(&srt, &errs, resolve_postfix_expr(resolver));
            break;
        case AST_IDENT_EXPR:
        case AST_INT_EXPR:
        case AST_CHAR_EXPR:
        case AST_STRING_EXPR:
            resolverret_assign(&srt, &errs, resolve_primary_expr(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_assignment_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

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
        err = new_error("left operand of assignment should be modifiable lvalue");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_isassignable(lhs_srt->dtype, rhs_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("expression is not assignable to lvalue");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_equals(rhs_srt->dtype, lhs_srt->dtype)) {
        rhs_srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(lhs_srt->dtype), 1, rhs_srt);
    }

    dtype = dtype_copy(lhs_srt->dtype);
    lhs_srt = convert_to_ptr(lhs_srt);

    srt = new_dtyped_srt(SRT_ASSIGN_EXPR, dtype, 2, lhs_srt, rhs_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_compound_assignment_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;
    Ast* lhs_ast = vector_at(ast->children, 0);
    Ast* rhs_ast = vector_at(ast->children, 1);

    Ast* binop_ast = NULL;
    switch (ast->type) {
        case AST_MUL_ASSIGN_EXPR:
            binop_ast = new_ast(AST_MUL_EXPR, 0);
            break;
        case AST_DIV_ASSIGN_EXPR:
            binop_ast = new_ast(AST_DIV_EXPR, 0);
            break;
        case AST_MOD_ASSIGN_EXPR:
            binop_ast = new_ast(AST_MOD_EXPR, 0);
            break;
        case AST_ADD_ASSIGN_EXPR:
            binop_ast = new_ast(AST_ADD_EXPR, 0);
            break;
        case AST_SUB_ASSIGN_EXPR:
            binop_ast = new_ast(AST_SUB_EXPR, 0);
            break;
        case AST_OR_ASSIGN_EXPR:
            binop_ast = new_ast(AST_OR_EXPR, 0);
            break;
        case AST_XOR_ASSIGN_EXPR:
            binop_ast = new_ast(AST_XOR_EXPR, 0);
            break;
        case AST_AND_ASSIGN_EXPR:
            binop_ast = new_ast(AST_AND_EXPR, 0);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_compound_assignment_expr)\n");
            exit(1);
    }

    vector_push(binop_ast->children, ast_copy(lhs_ast));
    vector_push(binop_ast->children, ast_copy(rhs_ast));

    resolver->ast = new_ast(AST_ASSIGN_EXPR, 2, ast_copy(lhs_ast), binop_ast);
    resolverret_assign(&srt, &errs, resolve_assignment_expr(resolver));
    delete_ast(resolver->ast);
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    return new_resolverret(srt);
}

ResolverReturn* resolve_conditional_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* condition_srt = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&condition_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    condition_srt = convert_to_ptr_if_array(condition_srt);
    condition_srt = convert_to_ptr_if_function(condition_srt);

    if (!dtype_isscalar(condition_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("condition of conditional expression should have scalar type");
        vector_push(errs, err);
        delete_srt(condition_srt);
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(condition_srt);
        return new_resolverret_errors(errs);
    }
    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    resolver->ast = vector_at(ast->children, 2);
    resolverret_assign(&rhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(condition_srt);
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }
    rhs_srt = convert_to_ptr_if_array(rhs_srt);
    rhs_srt = convert_to_ptr_if_function(rhs_srt);

    if (dtype_isarithmetic(lhs_srt->dtype) && dtype_isarithmetic(rhs_srt->dtype)) {
        Pair* srt_pair = new_pair(&t_srt, &t_srt);
        pair_set(srt_pair, lhs_srt, rhs_srt);
        pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));
    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER &&
               dtype_iscompatible(lhs_srt->dtype, rhs_srt->dtype)) {
        // do nothing
    } else if (lhs_srt->dtype->type == DTYPE_STRUCT && rhs_srt->dtype->type == DTYPE_STRUCT &&
               dtype_equals(lhs_srt->dtype, rhs_srt->dtype)) {
        // do nothing
    } else {
        errs = new_vector(&t_error);
        err = new_error("operands of conditional expression are not compatible");
        vector_push(errs, err);
        delete_srt(condition_srt);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    srt = new_dtyped_srt(SRT_COND_EXPR, dtype_copy(lhs_srt->dtype), 3, condition_srt, lhs_srt, rhs_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_logical_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

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

    switch (ast->type) {
        case AST_LOR_EXPR:
            if (!dtype_isscalar(lhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("each of operands of || should have scalar type");
                vector_push(errs, err);
            }
            if (!dtype_isscalar(rhs_srt->dtype)) {
                if (errs == NULL) {
                    errs = new_vector(&t_error);
                }
                err = new_error("each of operands of || should have scalar type");
                vector_push(errs, err);
            }
            if (errs == NULL) {
                dtype = new_integer_dtype(DTYPE_INT);
                srt = new_dtyped_srt(SRT_LOR_EXPR, dtype, 2, lhs_srt, rhs_srt);
            }
            break;
        case AST_LAND_EXPR:
            if (!dtype_isscalar(lhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("each of operands of && should have scalar type");
                vector_push(errs, err);
            }
            if (!dtype_isscalar(rhs_srt->dtype)) {
                if (errs == NULL) {
                    errs = new_vector(&t_error);
                }
                err = new_error("each of operands of && should have scalar type");
                vector_push(errs, err);
            }
            if (errs == NULL) {
                dtype = new_integer_dtype(DTYPE_INT);
                srt = new_dtyped_srt(SRT_LAND_EXPR, dtype, 2, lhs_srt, rhs_srt);
            }
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_logical_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_bitwise_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

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

    Pair* srt_pair = new_pair(&t_srt, &t_srt);
    pair_set(srt_pair, lhs_srt, rhs_srt);
    pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));

    switch (ast->type) {
        case AST_OR_EXPR:
            if (!dtype_isinteger(lhs_srt->dtype) || !dtype_isinteger(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("binary | expression should be integer | integer");
                vector_push(errs, err);
                break;
            }
            dtype = dtype_copy(lhs_srt->dtype);
            srt = new_dtyped_srt(SRT_OR_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_XOR_EXPR:
            if (!dtype_isinteger(lhs_srt->dtype) || !dtype_isinteger(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("binary ^ expression should be integer ^ integer");
                vector_push(errs, err);
                break;
            }
            dtype = dtype_copy(lhs_srt->dtype);
            srt = new_dtyped_srt(SRT_XOR_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_AND_EXPR:
            if (!dtype_isinteger(lhs_srt->dtype) || !dtype_isinteger(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("binary & expression should be integer & integer");
                vector_push(errs, err);
                break;
            }
            dtype = dtype_copy(lhs_srt->dtype);
            srt = new_dtyped_srt(SRT_AND_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_bitwise_expr)\n");
            exit(1);
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
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

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
        Pair* srt_pair = new_pair(&t_srt, &t_srt);
        pair_set(srt_pair, lhs_srt, rhs_srt);
        pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));
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
                    err = new_error("operands of pointer == pointer are not compatible");
                } else {
                    err = new_error("binary == expression should be "
                                    "either arithmetic == arithmetic or pointer == pointer");
                }
                vector_push(errs, err);
                break;
            }
            dtype = new_integer_dtype(DTYPE_INT);
            srt = new_dtyped_srt(SRT_EQUAL_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_NEQUAL_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("operands of pointer != pointer are not compatible");
                } else {
                    err = new_error("binary != expression should be "
                                    "either arithmetic != arithmetic or pointer != pointer");
                }
                vector_push(errs, err);
                break;
            }
            dtype = new_integer_dtype(DTYPE_INT);
            srt = new_dtyped_srt(SRT_NEQUAL_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_equality_expr)");
            exit(1);
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_relational_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

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
        Pair* srt_pair = new_pair(&t_srt, &t_srt);
        pair_set(srt_pair, lhs_srt, rhs_srt);
        pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));
    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER &&
               dtype_iscompatible(lhs_srt->dtype, rhs_srt->dtype)) {
        // do nothing
    } else {
        errs = new_vector(&t_error);
    }

    switch (ast->type) {
        case AST_LESS_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("operands of pointer < pointer are not compatible");
                } else {
                    err = new_error("binary < expression should be "
                                    "either arithmetic < arithmetic or pointer < pointer");
                }
                vector_push(errs, err);
                break;
            }
            dtype = new_integer_dtype(DTYPE_INT);
            srt = new_dtyped_srt(SRT_LESS_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_GREATER_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("operands of pointer > pointer are not compatible");
                } else {
                    err = new_error("binary > expression should be "
                                    "either arithmetic > arithmetic or pointer > pointer");
                }
                vector_push(errs, err);
                break;
            }
            dtype = new_integer_dtype(DTYPE_INT);
            srt = new_dtyped_srt(SRT_GREATER_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_LESSEQ_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("operands of pointer <= pointer are not compatible");
                } else {
                    err = new_error("binary <= expression should be "
                                    "either arithmetic <= arithmetic or pointer <= pointer");
                }
                vector_push(errs, err);
                break;
            }
            dtype = new_integer_dtype(DTYPE_INT);
            srt = new_dtyped_srt(SRT_LESSEQ_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_GREATEREQ_EXPR:
            if (errs != NULL) {
                if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
                    err = new_error("operands of pointer >= pointer are not compatible");
                } else {
                    err = new_error("binary >= expression should be "
                                    "either arithmetic >= arithmetic or pointer >= pointer");
                }
                vector_push(errs, err);
                break;
            }
            dtype = new_integer_dtype(DTYPE_INT);
            srt = new_dtyped_srt(SRT_GREATEREQ_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_relational_expr)\n");
            exit(1);
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
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_ADD_EXPR:
            resolverret_assign(&srt, &errs, resolve_add_expr(resolver));
            break;
        case AST_SUB_EXPR:
            resolverret_assign(&srt, &errs, resolve_subtract_expr(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_additive_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_add_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
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
        Pair* srt_pair = new_pair(&t_srt, &t_srt);
        pair_set(srt_pair, lhs_srt, rhs_srt);
        pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));
        dtype = dtype_copy(lhs_srt->dtype);
        srt = new_dtyped_srt(SRT_ADD_EXPR, dtype, 2, lhs_srt, rhs_srt);
        return new_resolverret(srt);
    } else if (lhs_srt->dtype->type == DTYPE_POINTER && dtype_isinteger(rhs_srt->dtype)) {
        dtype = dtype_copy(lhs_srt->dtype);
        srt = new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, lhs_srt, rhs_srt);
        return new_resolverret(srt);
    } else if (dtype_isinteger(lhs_srt->dtype) && rhs_srt->dtype->type == DTYPE_POINTER) {
        dtype = dtype_copy(rhs_srt->dtype);
        srt = new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, rhs_srt, lhs_srt);
        return new_resolverret(srt);
    }

    errs = new_vector(&t_error);
    err = new_error("binary + expression should be either arithmetic + arithmetic, "
                    "pointer + integer, or integer + pointer");
    vector_push(errs, err);

    delete_srt(lhs_srt);
    delete_srt(rhs_srt);
    return new_resolverret_errors(errs);
}

ResolverReturn* resolve_subtract_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
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
        Pair* srt_pair = new_pair(&t_srt, &t_srt);
        pair_set(srt_pair, lhs_srt, rhs_srt);
        pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));
        dtype = dtype_copy(lhs_srt->dtype);
        srt = new_dtyped_srt(SRT_SUB_EXPR, dtype, 2, lhs_srt, rhs_srt);
        return new_resolverret(srt);
    } else if (lhs_srt->dtype->type == DTYPE_POINTER && dtype_isinteger(rhs_srt->dtype)) {
        dtype = dtype_copy(lhs_srt->dtype);
        srt = new_dtyped_srt(SRT_PSUB_EXPR, dtype, 2, lhs_srt, rhs_srt);
        return new_resolverret(srt);
    } else if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER &&
               dtype_iscompatible(lhs_srt->dtype, rhs_srt->dtype)) {
        dtype = new_integer_dtype(DTYPE_UNSIGNED_LONGLONG);
        srt = new_dtyped_srt(SRT_PDIFF_EXPR, dtype, 2, lhs_srt, rhs_srt);
        return new_resolverret(srt);
    }

    errs = new_vector(&t_error);
    if (lhs_srt->dtype->type == DTYPE_POINTER && rhs_srt->dtype->type == DTYPE_POINTER) {
        err = new_error("operands of pointer - pointer are not compatible");
    } else {
        err = new_error("binary - expression should be either arithmetic - arithmetic, "
                        "pointer - integer, or pointer - pointer");
    }
    vector_push(errs, err);

    delete_srt(lhs_srt);
    delete_srt(rhs_srt);
    return new_resolverret_errors(errs);
}

ResolverReturn* resolve_multiplicative_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
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

    Pair* srt_pair = new_pair(&t_srt, &t_srt);
    pair_set(srt_pair, lhs_srt, rhs_srt);
    pair_assign((void**)&lhs_srt, (void**)&rhs_srt, perform_usual_arithmetic_conversion(srt_pair));

    switch (ast->type) {
        case AST_MUL_EXPR:
            if (!dtype_isarithmetic(lhs_srt->dtype) || !dtype_isarithmetic(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("binary * expression should be arithmetic * arithmetic");
                vector_push(errs, err);
                break;
            }
            dtype = dtype_copy(lhs_srt->dtype);
            srt = new_dtyped_srt(SRT_MUL_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_DIV_EXPR:
            if (!dtype_isarithmetic(lhs_srt->dtype) || !dtype_isarithmetic(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("binary / expression should be arithmetic / arithmetic");
                vector_push(errs, err);
                break;
            }
            dtype = dtype_copy(lhs_srt->dtype);
            srt = new_dtyped_srt(SRT_DIV_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        case AST_MOD_EXPR:
            if (!dtype_isinteger(lhs_srt->dtype) || !dtype_isinteger(rhs_srt->dtype)) {
                errs = new_vector(&t_error);
                err = new_error("binary %% expression should be integer %% integer");
                vector_push(errs, err);
                break;
            }
            dtype = dtype_copy(lhs_srt->dtype);
            srt = new_dtyped_srt(SRT_MOD_EXPR, dtype, 2, lhs_srt, rhs_srt);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_multiplicative_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_cast_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&dtype, &errs, resolve_type_name(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_dtype(dtype);
        return new_resolverret_errors(errs);
    }

    if (dtype->type != DTYPE_VOID && (!dtype_isscalar(dtype) || !dtype_isscalar(child_srt->dtype))) {
        errs = new_vector(&t_error);
        err = new_error("cast should be from an any type to void or from a scalar type to scalar type");
        vector_push(errs, err);
        delete_dtype(dtype);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    srt = new_dtyped_srt(SRT_CAST_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_unary_expr(Resolver* resolver) {
    ResolverReturn* resolve_incdec_expr(Resolver * resolver, SrtType srt_type, char* op);
    ResolverReturn* resolve_address_expr(Resolver * resolver);
    ResolverReturn* resolve_indirection_expr(Resolver * resolver);
    ResolverReturn* resolve_sign_expr(Resolver * resolver, SrtType srt_type, char op);
    ResolverReturn* resolve_bitwise_not_expr(Resolver * resolver);
    ResolverReturn* resolve_logical_not_expr(Resolver * resolver);
    ResolverReturn* resolve_sizeof_expr(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_PREINC_EXPR:
            resolverret_assign(&srt, &errs, resolve_incdec_expr(resolver, SRT_PREINC_EXPR, "++"));
            break;
        case AST_PREDEC_EXPR:
            resolverret_assign(&srt, &errs, resolve_incdec_expr(resolver, SRT_PREDEC_EXPR, "--"));
            break;
        case AST_ADDR_EXPR:
            resolverret_assign(&srt, &errs, resolve_address_expr(resolver));
            break;
        case AST_INDIR_EXPR:
            resolverret_assign(&srt, &errs, resolve_indirection_expr(resolver));
            break;
        case AST_POS_EXPR:
            resolverret_assign(&srt, &errs, resolve_sign_expr(resolver, SRT_POS_EXPR, '+'));
            break;
        case AST_NEG_EXPR:
            resolverret_assign(&srt, &errs, resolve_sign_expr(resolver, SRT_NEG_EXPR, '-'));
            break;
        case AST_NOT_EXPR:
            resolverret_assign(&srt, &errs, resolve_bitwise_not_expr(resolver));
            break;
        case AST_LNOT_EXPR:
            resolverret_assign(&srt, &errs, resolve_logical_not_expr(resolver));
            break;
        case AST_SIZEOF_EXPR:
            resolverret_assign(&srt, &errs, resolve_sizeof_expr(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_unary_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_address_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    if (child_srt->dtype->type != DTYPE_FUNCTION && child_srt->type != SRT_INDIR_EXPR &&
        (child_srt->type != SRT_IDENT_EXPR || !dtype_isobject(child_srt->dtype))) {
        errs = new_vector(&t_error);
        err = new_error("operand of unary & is neither a function designator, "
                        "a indirection, nor an object lvalue");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    dtype = new_pointer_dtype(dtype_copy(child_srt->dtype));
    srt = new_dtyped_srt(SRT_ADDR_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_indirection_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);

    if (child_srt->dtype->type != DTYPE_POINTER) {
        errs = new_vector(&t_error);
        err = new_error("operand of unary * does not have pointer type");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    dtype = dtype_copy(child_srt->dtype->dpointer->to_dtype);
    srt = new_dtyped_srt(SRT_INDIR_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_sign_expr(Resolver* resolver, SrtType srt_type, char op) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    if (!dtype_isarithmetic(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("operand of unary %c does not have arithmetic type", op);
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    child_srt = perform_integer_promotion(child_srt);

    dtype = dtype_copy(child_srt->dtype);
    srt = new_dtyped_srt(srt_type, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_bitwise_not_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    if (!dtype_isinteger(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("operand of unary ~ does not have integer type");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    child_srt = perform_integer_promotion(child_srt);

    dtype = dtype_copy(child_srt->dtype);
    srt = new_dtyped_srt(SRT_NOT_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_logical_not_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);

    if (!dtype_isscalar(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("operand of unary ! does not have scalar type");
        vector_push(errs, err);
        delete_srt(child_srt);
        return new_resolverret_errors(errs);
    }

    dtype = new_integer_dtype(DTYPE_INT);
    srt = new_dtyped_srt(SRT_LNOT_EXPR, dtype, 1, child_srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_sizeof_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Srt* child_srt = NULL;
    DType* child_dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    if (resolver->ast->type == AST_TYPE_NAME) {
        resolverret_dtype_assign(&child_dtype, &errs, resolve_type_name(resolver));
    } else {
        resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    }
    resolver->ast = ast;

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    if (child_dtype == NULL) {
        child_dtype = dtype_copy(child_srt->dtype);
        delete_srt(child_srt);
    }

    if (child_dtype->type == DTYPE_FUNCTION || dtype_isincomplete(child_dtype)) {
        errs = new_vector(&t_error);
        err = new_error("operand of sizeof has neither function type nor an incomplete type");
        vector_push(errs, err);
        delete_dtype(child_dtype);
        return new_resolverret_errors(errs);
    }

    DType* dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, dtype_nbytes(child_dtype));
    delete_dtype(child_dtype);

    srt = new_iliteral_srt(SRT_INT_EXPR, dtype, iliteral);
    return new_resolverret(srt);
}

ResolverReturn* resolve_postfix_expr(Resolver* resolver) {
    ResolverReturn* resolve_subscription_expr(Resolver * resolver);
    ResolverReturn* resolve_call_expr(Resolver * resolver);
    ResolverReturn* resolve_member_like_expr(Resolver * resolver);
    ResolverReturn* resolve_incdec_expr(Resolver * resolver, SrtType srt_type, char* op);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_SUBSC_EXPR:
            resolverret_assign(&srt, &errs, resolve_subscription_expr(resolver));
            break;
        case AST_CALL_EXPR:
            resolverret_assign(&srt, &errs, resolve_call_expr(resolver));
            break;
        case AST_MEMBER_EXPR:
        case AST_TOMEMBER_EXPR:
            resolverret_assign(&srt, &errs, resolve_member_like_expr(resolver));
            break;
        case AST_POSTINC_EXPR:
            resolverret_assign(&srt, &errs, resolve_incdec_expr(resolver, SRT_POSTINC_EXPR, "++"));
            break;
        case AST_POSTDEC_EXPR:
            resolverret_assign(&srt, &errs, resolve_incdec_expr(resolver, SRT_POSTDEC_EXPR, "--"));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_postfix_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_subscription_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

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
        err = new_error("subscription should be pointer[integer] or integer[pointer]");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    if (rhs_srt->dtype->type == DTYPE_POINTER) {
        Srt* tmp_srt = lhs_srt;
        lhs_srt = rhs_srt;
        rhs_srt = tmp_srt;
    }

    if (!dtype_isobject(lhs_srt->dtype->dpointer->to_dtype)) {
        errs = new_vector(&t_error);
        err = new_error("subscribed object should have pointer to object type");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        delete_srt(rhs_srt);
        return new_resolverret_errors(errs);
    }

    dtype = dtype_copy(lhs_srt->dtype);
    srt = new_dtyped_srt(SRT_PADD_EXPR, dtype, 2, lhs_srt, rhs_srt);
    srt = new_dtyped_srt(SRT_INDIR_EXPR, dtype_copy(lhs_srt->dtype->dpointer->to_dtype), 1, srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_call_expr(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    lhs_srt = convert_to_ptr_if_array(lhs_srt);
    lhs_srt = convert_to_ptr_if_function(lhs_srt);

    if (lhs_srt->dtype->type != DTYPE_POINTER || lhs_srt->dtype->dpointer->to_dtype->type != DTYPE_FUNCTION) {
        errs = new_vector(&t_error);
        err = new_error("called object is not a function or a function pointer");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    DType* original_call_dtype = resolver->expr_dtype;
    resolver->expr_dtype = lhs_srt->dtype->dpointer->to_dtype;
    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_argument_expr_list(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        resolver->expr_dtype = original_call_dtype;
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    dtype = dtype_copy(resolver->expr_dtype->dfunction->return_dtype);
    srt = new_dtyped_srt(SRT_CALL_EXPR, dtype, 2, lhs_srt, rhs_srt);

    resolver->expr_dtype = original_call_dtype;
    return new_resolverret(srt);
}

ResolverReturn* resolve_member_like_expr(Resolver* resolver) {
    ResolverReturn* resolve_member_name_expr(Resolver * resolver);

    Srt* srt = NULL;
    DType* dtype = NULL;
    Srt* lhs_srt = NULL;
    Srt* rhs_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&lhs_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    if (ast->type == AST_MEMBER_EXPR && lhs_srt->dtype->type != DTYPE_STRUCT) {
        errs = new_vector(&t_error);
        err = new_error("dot-accessed object is not a struct");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    if (ast->type == AST_TOMEMBER_EXPR &&
        (lhs_srt->dtype->type != DTYPE_POINTER || lhs_srt->dtype->dpointer->to_dtype->type != DTYPE_STRUCT)) {
        errs = new_vector(&t_error);
        err = new_error("arrow-accessed object is not a pointer to a struct");
        vector_push(errs, err);
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    if (lhs_srt->dtype->type == DTYPE_STRUCT) {
        DType* lhs_dtype = new_pointer_dtype(dtype_copy(lhs_srt->dtype));
        lhs_srt = new_dtyped_srt(SRT_ADDR_EXPR, lhs_dtype, 1, lhs_srt);
    }

    DType* original_member_dtype = resolver->expr_dtype;
    resolver->expr_dtype = lhs_srt->dtype->dpointer->to_dtype;
    if (resolver->expr_dtype->dstruct->members == NULL) {
        resolver->expr_dtype = tagtable_search(resolver->tag_table, resolver->expr_dtype->dstruct->name);
    }

    if (resolver->expr_dtype == NULL) {
        errs = new_vector(&t_error);
        err = new_error("struct '%s' is incomplete", lhs_srt->dtype->dpointer->to_dtype->dstruct->name);
        vector_push(errs, err);
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&rhs_srt, &errs, resolve_member_name_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        resolver->expr_dtype = original_member_dtype;
        delete_srt(lhs_srt);
        return new_resolverret_errors(errs);
    }

    dtype = dtype_copy(rhs_srt->dtype);
    srt = new_dtyped_srt(SRT_TOMEMBER_EXPR, dtype, 2, lhs_srt, rhs_srt);

    resolver->expr_dtype = original_member_dtype;
    return new_resolverret(srt);
}

ResolverReturn* resolve_incdec_expr(Resolver* resolver, SrtType srt_type, char* op) {
    Srt* srt = NULL;
    DType* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    if (!srt_ismodifiable(srt)) {
        errs = new_vector(&t_error);
        err = new_error("operand of %s is not modifiable", op);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_isinteger(srt->dtype) && srt->dtype->type != DTYPE_POINTER) {
        errs = new_vector(&t_error);
        err = new_error("operand of %s should be either integer or pointer", op);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    dtype = dtype_copy(srt->dtype);
    srt = convert_to_ptr(srt);
    srt = new_dtyped_srt(srt_type, dtype, 1, srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_argument_expr_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_ARG_LIST, 0);
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;
    DType* call_dtype = resolver->expr_dtype;

    int num_params = vector_size(call_dtype->dfunction->params);
    int num_args = vector_size(ast->children);

    if (num_params != num_args) {
        errs = new_vector(&t_error);
        err = new_error("function takes %d params, but passed %d arguments", num_params, num_args);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    for (int i = 0; i < num_args; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;
        DParam* dparam = vector_at(call_dtype->dfunction->params, i);

        resolver->ast = vector_at(ast->children, i);
        resolverret_assign(&child_srt, &child_errs, resolve_expr(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        }

        child_srt = convert_to_ptr_if_array(child_srt);
        child_srt = convert_to_ptr_if_function(child_srt);

        if (!dtype_isassignable(dparam->dtype, child_srt->dtype)) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            err = new_error("argument is not assignable to parameter");
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

ResolverReturn* resolve_member_name_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;
    DType* member_dtype = resolver->expr_dtype;

    int num_members = vector_size(member_dtype->dstruct->members);
    for (int i = 0; i < num_members; i++) {
        DStructMember* member = vector_at(member_dtype->dstruct->members, i);
        if (strcmp(member->name, ast->ident_name) != 0) {
            continue;
        }

        srt = new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(member->dtype), new_string(member->name));
        break;
    }

    if (srt == NULL) {
        errs = new_vector(&t_error);
        err = new_error("member '%s' does not exist in struct", ast->ident_name);
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    return new_resolverret(srt);
}

ResolverReturn* resolve_primary_expr(Resolver* resolver) {
    ResolverReturn* resolve_identifier_expr(Resolver * resolver);
    ResolverReturn* resolve_integer_expr(Resolver * resolver);
    ResolverReturn* resolve_character_expr(Resolver * resolver);
    ResolverReturn* resolve_string_expr(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_IDENT_EXPR:
            resolverret_assign(&srt, &errs, resolve_identifier_expr(resolver));
            break;
        case AST_INT_EXPR:
            resolverret_assign(&srt, &errs, resolve_integer_expr(resolver));
            break;
        case AST_CHAR_EXPR:
            resolverret_assign(&srt, &errs, resolve_character_expr(resolver));
            break;
        case AST_STRING_EXPR:
            resolverret_assign(&srt, &errs, resolve_string_expr(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_primary_expr)\n");
            exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_identifier_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    Symbol* symbol = symboltable_search(resolver->symbol_table, ast->ident_name);
    if (symbol == NULL) {
        errs = new_vector(&t_error);
        err = new_error("identifier '%s' is used before declared", ast->ident_name);
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    if (symbol->type == SYMBOL_INT) {
        DType* dtype = new_integer_dtype(DTYPE_INT);
        IntegerLiteral* iliteral = iliteral_copy(symbol->iliteral);
        srt = new_iliteral_srt(SRT_INT_EXPR, dtype, iliteral);
        return new_resolverret(srt);
    }

    DType* ident_dtype = dtype_copy(symbol->dtype);
    char* ident_name = new_string(symbol->name);
    srt = new_identifier_srt(SRT_IDENT_EXPR, ident_dtype, ident_name);
    return new_resolverret(srt);
}

ResolverReturn* resolve_integer_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->ast;

    DType* dtype = NULL;
    switch (ast->iliteral->type) {
        case INTEGER_INT:
            dtype = new_integer_dtype(DTYPE_INT);
            break;
        case INTEGER_UNSIGNED_INT:
            dtype = new_integer_dtype(DTYPE_UNSIGNED_INT);
            break;
        case INTEGER_LONG:
            dtype = new_integer_dtype(DTYPE_LONG);
            break;
        case INTEGER_UNSIGNED_LONG:
            dtype = new_integer_dtype(DTYPE_UNSIGNED_LONG);
            break;
        case INTEGER_LONGLONG:
            dtype = new_integer_dtype(DTYPE_LONGLONG);
            break;
        case INTEGER_UNSIGNED_LONGLONG:
            dtype = new_integer_dtype(DTYPE_UNSIGNED_LONGLONG);
            break;
    }

    IntegerLiteral* iliteral = iliteral_copy(ast->iliteral);
    srt = new_iliteral_srt(SRT_INT_EXPR, dtype, iliteral);
    return new_resolverret(srt);
}

ResolverReturn* resolve_character_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->ast;

    DType* dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* iliteral = iliteral_copy(ast->iliteral);
    srt = new_iliteral_srt(SRT_CHAR_EXPR, dtype, iliteral);
    return new_resolverret(srt);
}

ResolverReturn* resolve_string_expr(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->ast;

    resolver->sliteral_id++;
    DType* decl_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), ast->sliteral->size);
    int sliteral_id = resolver->sliteral_id;
    Srt* decl_srt = new_sliteral_identifier_srt(SRT_STRDECL, decl_dtype, sliteral_id);

    DType* init_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), ast->sliteral->size);
    StringLiteral* sliteral = sliteral_copy(ast->sliteral);
    Srt* literal_srt = new_sliteral_srt(SRT_STRING_EXPR, init_dtype, sliteral);
    Srt* init_srt = new_srt(SRT_INIT, 1, literal_srt);

    Srt* decl_list_srt = new_srt(SRT_DECL_LIST, 1, new_srt(SRT_INIT_DECL, 2, decl_srt, init_srt));
    vector_push(resolver->trans_unit_srt->children, decl_list_srt);

    DType* ident_dtype = dtype_copy(decl_dtype);
    srt = new_sliteral_identifier_srt(SRT_STRIDENT_EXPR, ident_dtype, sliteral_id);

    return new_resolverret(srt);
}
