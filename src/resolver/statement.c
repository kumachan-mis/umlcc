#include "./statement.h"
#include "./conversion.h"
#include "./declaration.h"
#include "./expression.h"

#include <stdio.h>
#include <stdlib.h>

ResolverReturn* resolve_stmt(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    switch (ast->type) {
        case AST_CMPD_STMT:
            resolver->symbol_table = symboltable_enter_scope(resolver->symbol_table);
            resolver->tag_table = tagtable_enter_scope(resolver->tag_table);
            resolverret_assign(&srt, &errs, resolve_compound_stmt(resolver));
            resolver->tag_table = tagtable_exit_scope(resolver->tag_table);
            resolver->symbol_table = symboltable_exit_scope(resolver->symbol_table);
            break;
        case AST_CASE_STMT:
            resolverret_assign(&srt, &errs, resolve_case_stmt(resolver));
            break;
        case AST_DEFAULT_STMT:
            resolverret_assign(&srt, &errs, resolve_default_stmt(resolver));
            break;
        case AST_CONTINUE_STMT:
            resolverret_assign(&srt, &errs, resolve_continue_stmt(resolver));
            break;
        case AST_BREAK_STMT:
            resolverret_assign(&srt, &errs, resolve_break_stmt(resolver));
            break;
        case AST_RET_STMT:
            resolverret_assign(&srt, &errs, resolve_return_stmt(resolver));
            break;
        case AST_EXPR_STMT:
            resolverret_assign(&srt, &errs, resolve_expression_stmt(resolver));
            break;
        case AST_NULL_STMT:
            resolverret_assign(&srt, &errs, resolve_null_stmt());
            break;
        case AST_IF_STMT:
            resolverret_assign(&srt, &errs, resolve_if_else_stmt(resolver));
            break;
        case AST_SWITCH_STMT:
            resolverret_assign(&srt, &errs, resolve_switch_stmt(resolver));
            break;
        case AST_WHILE_STMT:
            resolverret_assign(&srt, &errs, resolve_while_stmt(resolver));
            break;
        case AST_FOR_STMT:
            resolver->symbol_table = symboltable_enter_scope(resolver->symbol_table);
            resolver->tag_table = tagtable_enter_scope(resolver->tag_table);
            resolverret_assign(&srt, &errs, resolve_for_stmt(resolver));
            resolver->symbol_table = symboltable_exit_scope(resolver->symbol_table);
            resolver->tag_table = tagtable_exit_scope(resolver->tag_table);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_stmt)\n");
            exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_case_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_CASE_STMT, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    if (resolver->switch_cases == NULL) {
        errs = new_vector(&t_error);
        err = new_error("case statement is not allowed outside of switch");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (child_srt->type != SRT_ILITERAL_EXPR || iliteral_isunsigned(child_srt->iliteral)) {
        errs = new_vector(&t_error);
        err = new_error("only signed integer constant is supported as case label");
        vector_push(errs, err);
        delete_srt(child_srt);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    child_srt = perform_integer_promotion(child_srt);
    vector_push(srt->children, child_srt);

    IntegerLiteral* case_value_iliteral = child_srt->iliteral;
    int num_cases = vector_size(resolver->switch_cases);
    for (int i = 0; i < num_cases; i++) {
        IntegerLiteral* iliteral = vector_at(resolver->switch_cases, i);
        if (iliteral->signed_value != case_value_iliteral->signed_value) {
            continue;
        }
        errs = new_vector(&t_error);
        err = new_error("value of case label is already used in switch");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(resolver->switch_cases, iliteral_copy(case_value_iliteral));

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_default_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_DEFAULT_STMT, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    if (resolver->switch_cases == NULL) {
        errs = new_vector(&t_error);
        err = new_error("default statement is not allowed outside of switch");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (resolver->switch_default_exists) {
        errs = new_vector(&t_error);
        err = new_error("default statement is already used in switch");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    resolver->switch_default_exists = 1;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_compound_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_CMPD_STMT, 0);
    Vector* errs = NULL;
    Ast* ast = resolver->ast;
    Srt* scope_srt = resolver->scope_srt;

    resolver->scope_srt = srt;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        resolver->ast = vector_at(ast->children, i);
        if (resolver->ast->type == AST_DECL) {
            resolverret_assign(&child_srt, &child_errs, resolve_decl(resolver));
        } else {
            resolverret_assign(&child_srt, &child_errs, resolve_stmt(resolver));
        }

        if (child_errs != NULL) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        } else if (errs != NULL) {
            delete_srt(child_srt);
            continue;
        }

        vector_push(srt->children, child_srt);
    }

    resolver->scope_srt = scope_srt;
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_return_stmt(Resolver* resolver) {
    Ast* ast = resolver->ast;
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    int num_children = vector_size(ast->children);
    if (resolver->return_dtype->type == DTYPE_VOID && num_children > 0) {
        errs = new_vector(&t_error);
        err = new_error("function returning void returns a value");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    } else if (resolver->return_dtype->type != DTYPE_VOID && num_children == 0) {
        errs = new_vector(&t_error);
        err = new_error("function returning non-void returns no value");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    if (num_children == 0) {
        srt = new_srt(SRT_RET_STMT, 0);
        return new_resolverret(srt);
    }

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    srt = convert_to_ptr_if_array(srt);
    srt = convert_to_ptr_if_function(srt);

    if (!dtype_isassignable(srt->dtype, resolver->return_dtype)) {
        errs = new_vector(&t_error);
        err = new_error("expression is not assignable to function return");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_equals(srt->dtype, resolver->return_dtype)) {
        srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(resolver->return_dtype), 1, srt);
    }
    srt = new_srt(SRT_RET_STMT, 1, srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_continue_stmt(Resolver* resolver) {
    if (!resolver->inside_loop) {
        Vector* errs = new_vector(&t_error);
        Error* err = new_error("continue statement is not allowed outside of loop");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    Srt* srt = new_srt(SRT_CONTINUE_STMT, 0);
    return new_resolverret(srt);
}

ResolverReturn* resolve_break_stmt(Resolver* resolver) {
    if (!resolver->inside_loop && resolver->switch_cases == NULL) {
        Vector* errs = new_vector(&t_error);
        Error* err = new_error("break statement is not allowed outside of loop or switch");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    Srt* srt = new_srt(SRT_BREAK_STMT, 0);
    return new_resolverret(srt);
}

ResolverReturn* resolve_expression_stmt(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    srt = convert_to_ptr_if_array(srt);
    srt = convert_to_ptr_if_function(srt);

    srt = new_srt(SRT_EXPR_STMT, 1, srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_null_stmt(void) {
    Srt* srt = new_srt(SRT_NULL_STMT, 0);
    return new_resolverret(srt);
}

ResolverReturn* resolve_if_else_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_IF_STMT, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);
    vector_push(srt->children, child_srt);

    if (!dtype_isscalar(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("condition of if-else statement should have scalar type");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    if (vector_size(ast->children) == 2) {
        return new_resolverret(srt);
    }

    resolver->ast = vector_at(ast->children, 2);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_switch_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_SWITCH_STMT, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);
    child_srt = perform_integer_promotion(child_srt);
    vector_push(srt->children, child_srt);

    if (!dtype_isinteger(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("condition of switch statement should have integer type");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    Vector* original_switch_cases = resolver->switch_cases;
    int original_switch_default_exists = resolver->switch_default_exists;

    resolver->switch_cases = new_vector(&t_iliteral);
    resolver->switch_default_exists = 0;

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;

    delete_vector(resolver->switch_cases);
    resolver->switch_cases = original_switch_cases;
    resolver->switch_default_exists = original_switch_default_exists;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_while_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_WHILE_STMT, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    child_srt = convert_to_ptr_if_array(child_srt);
    child_srt = convert_to_ptr_if_function(child_srt);
    vector_push(srt->children, child_srt);

    if (!dtype_isscalar(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("condition of while statement should have scalar type");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    int original_inside_loop = resolver->inside_loop;

    resolver->inside_loop = 1;
    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;
    resolver->inside_loop = original_inside_loop;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_for_stmt(Resolver* resolver) {
    Srt* srt = new_srt(SRT_FOR_STMT, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    int decl_init = resolver->ast->type == AST_DECL;
    if (decl_init) {
        resolverret_assign(&child_srt, &errs, resolve_decl(resolver));
    } else {
        resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    }
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(srt->children, child_srt);

    if (decl_init) {
        int num_children = vector_size(child_srt->children);
        for (int i = 0; i < num_children; i++) {
            Srt* init_decl_srt = vector_at(child_srt->children, i);
            Srt* decl_srt = vector_at(init_decl_srt->children, 0);
            if (decl_srt->dtype->type != DTYPE_TYPEDEF) {
                continue;
            }

            errs = new_vector(&t_error);
            err = new_error("typedef in for statement initializer is not allowed");
            vector_push(errs, err);
            delete_srt(srt);
            return new_resolverret_errors(errs);
        }
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(srt->children, child_srt);

    if (child_srt->type == SRT_EXPR_STMT) {
        Srt* controling_expr_srt = vector_at(child_srt->children, 0);
        if (!dtype_isscalar(controling_expr_srt->dtype)) {
            errs = new_vector(&t_error);
            err = new_error("condition of for statement should have scalar type");
            vector_push(errs, err);
            delete_srt(srt);
            return new_resolverret_errors(errs);
        }
    }

    resolver->ast = vector_at(ast->children, 2);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(srt->children, child_srt);

    int original_inside_loop = resolver->inside_loop;

    resolver->inside_loop = 1;
    resolver->ast = vector_at(ast->children, 3);
    resolverret_assign(&child_srt, &errs, resolve_stmt(resolver));
    resolver->ast = ast;
    resolver->inside_loop = original_inside_loop;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}
