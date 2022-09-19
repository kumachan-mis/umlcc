#include "./external.h"
#include "../common/type.h"
#include "./declaration.h"
#include "./statement.h"

#include <stdlib.h>

ResolverReturn* resolve_transration_unit(Resolver* resolver) {
    Srt* srt = new_srt(SRT_TRAS_UNIT, 0);
    Vector* errs = NULL;

    Ast* ast = resolver->ast;

    resolver->trans_unit_srt = srt;
    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        resolver->ast = vector_at(ast->children, i);
        if (resolver->ast->type == AST_FUNC_DEF) {
            resolverret_assign(&child_srt, &child_errs, resolve_function_definition(resolver));
        } else {
            resolverret_assign(&child_srt, &child_errs, resolve_decl(resolver));
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

    resolver->ast = ast;
    resolver->trans_unit_srt = NULL;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_function_definition(Resolver* resolver) {
    Srt* srt = new_srt(SRT_FUNC_DEF, 0);
    Dtype* specifiers_dtype = NULL;
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&specifiers_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (specifiers_dtype->type == DTYPE_DECORATION && specifiers_dtype->decoration->typedef_flag) {
        errs = new_vector(&t_error);
        err = new_error("Error: storage specifiers are invalid for a function definition\n");
        vector_push(errs, err);
        delete_dtype(specifiers_dtype);
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_declarator(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    child_srt->dtype = dtype_connect(child_srt->dtype, specifiers_dtype);

    if (child_srt->dtype->type != DTYPE_FUNCTION) {
        errs = new_vector(&t_error);
        err = new_error("Error: declaration of %s should not have body like a function\n",
                        dtype_types[child_srt->dtype->type]);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (!symboltable_can_define(resolver->global_table, child_srt->ident_name)) {
        errs = new_vector(&t_error);
        err = new_error("Error: identifier '%s' is already declared\n", child_srt->ident_name);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    char* symbol_name = new_string(child_srt->ident_name);
    Dtype* symbol_dtype = dtype_copy(child_srt->dtype);
    symboltable_define_label(resolver->global_table, symbol_name, symbol_dtype);

    vector_push(srt->children, child_srt);

    resolver->local_table = new_symboltable();
    resolver->return_dtype = symbol_dtype->function->return_dtype;

    Vector* params = child_srt->dtype->function->params;
    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        DParam* dparam = vector_at(params, i);
        char* symbol_name = new_string(dparam->ident_name);
        Dtype* symbol_dtype = dtype_copy(dparam->dtype);
        symboltable_define_memory(resolver->local_table, symbol_name, symbol_dtype);
    }

    resolver->ast = vector_at(ast->children, 2);
    resolverret_assign(&child_srt, &errs, resolve_compound_stmt(resolver));
    resolver->ast = ast;

    delete_symboltable(resolver->local_table);
    resolver->local_table = NULL;
    resolver->return_dtype = NULL;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}
