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
    resolver->scope_srt = srt;

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

    resolver->scope_srt = NULL;
    resolver->trans_unit_srt = NULL;
    resolver->ast = ast;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_function_definition(Resolver* resolver) {
    Srt* srt = NULL;
    DType* specifiers_dtype = NULL;
    Srt* declarator_srt = NULL;
    Srt* body_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&specifiers_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_errors(errs);

    if (specifiers_dtype->type == DTYPE_DECORATION && specifiers_dtype->ddecoration->typedef_flag) {
        errs = new_vector(&t_error);
        err = new_error("storage specifiers are invalid for a function definition\n");
        vector_push(errs, err);
        delete_dtype(specifiers_dtype);
        return new_resolverret_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&declarator_srt, &errs, resolve_declarator(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_dtype(specifiers_dtype);
        return new_resolverret_errors(errs);
    }

    declarator_srt->dtype = dtype_connect(declarator_srt->dtype, specifiers_dtype);

    if (declarator_srt->dtype->type != DTYPE_FUNCTION) {
        errs = new_vector(&t_error);
        err = new_error("non-function declaration should not have body\n");
        vector_push(errs, err);
        delete_srt(declarator_srt);
        return new_resolverret_errors(errs);
    }

    if (!symboltable_can_define(resolver->symbol_table, declarator_srt->ident_name)) {
        errs = new_vector(&t_error);
        err = new_error("identifier '%s' is already declared\n", declarator_srt->ident_name);
        vector_push(errs, err);
        delete_srt(declarator_srt);
        return new_resolverret_errors(errs);
    }

    char* symbol_name = new_string(declarator_srt->ident_name);
    DType* symbol_dtype = dtype_copy(declarator_srt->dtype);
    symboltable_define_label(resolver->symbol_table, symbol_name, symbol_dtype);

    resolver->symbol_table = symboltable_enter_scope(resolver->symbol_table);
    resolver->symbol_table->memory_nbytes = 0;
    resolver->tag_table = tagtable_enter_scope(resolver->tag_table);
    resolver->return_dtype = symbol_dtype->dfunction->return_dtype;

    Vector* params = declarator_srt->dtype->dfunction->params;
    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        DParam* dparam = vector_at(params, i);
        char* symbol_name = new_string(dparam->name);
        DType* symbol_dtype = dtype_copy(dparam->dtype);
        symboltable_define_memory(resolver->symbol_table, symbol_name, symbol_dtype);
    }

    resolver->ast = vector_at(ast->children, 2);
    resolverret_assign(&body_srt, &errs, resolve_compound_stmt(resolver));
    resolver->ast = ast;

    resolver->return_dtype = NULL;
    resolver->tag_table = tagtable_exit_scope(resolver->tag_table);
    resolver->symbol_table->memory_nbytes = 0;
    resolver->symbol_table = symboltable_exit_scope(resolver->symbol_table);

    if (errs != NULL) {
        delete_srt(declarator_srt);
        return new_resolverret_errors(errs);
    }

    srt = new_srt(SRT_FUNC_DEF, 2, declarator_srt, body_srt);
    return new_resolverret(srt);
}
