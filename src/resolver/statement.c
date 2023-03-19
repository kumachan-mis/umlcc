#include "./statement.h"
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
        case AST_RET_STMT:
            resolverret_assign(&srt, &errs, resolve_return_stmt(resolver));
            break;
        case AST_EXPR_STMT:
            resolverret_assign(&srt, &errs, resolve_expression_stmt(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_stmt)\n");
            exit(1);
    }

    if (errs != NULL) return new_resolverret_errors(errs);
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
            if (errs == NULL) errs = new_vector(&t_error);
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
        err = new_error("function returning void returns a value\n");
        return new_resolverret_errors(errs);
    } else if (resolver->return_dtype->type != DTYPE_VOID && num_children == 0) {
        errs = new_vector(&t_error);
        err = new_error("function returning non-void returns no value\n");
        return new_resolverret_errors(errs);
    }

    if (num_children == 0) {
        srt = new_srt(SRT_RET_STMT, 0);
        return new_resolverret(srt);
    }

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    if (!dtype_isassignable(srt->dtype, resolver->return_dtype)) {
        errs = new_vector(&t_error);
        err = new_error("expression is not assignable to function return\n");
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

ResolverReturn* resolve_expression_stmt(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    srt = new_srt(SRT_EXPR_STMT, 1, srt);
    return new_resolverret(srt);
}
