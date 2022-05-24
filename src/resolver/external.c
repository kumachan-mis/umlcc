#include "./external.h"
#include "../common/common.h"
#include "./declaration.h"
#include "./statement.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_transration_unit(Resolver* resolver) {
    Srt* srt = new_srt(SRT_TRAS_UNIT, 0);
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->ast = vector_at(ast->children, i);
        if (resolver->ast->type == AST_FUNC_DEF) {
            vector_push(srt->children, resolve_function_definition(resolver));
        } else {
            vector_push(srt->children, resolve_decl(resolver));
        }
    }

    resolver->ast = ast;
    return srt;
}

Srt* resolve_function_definition(Resolver* resolver) {
    Srt* srt = new_srt(SRT_FUNC_DEF, 0);
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolver->shared_dtype = resolve_decl_specifiers(resolver);

    resolver->ast = vector_at(ast->children, 1);
    Srt* declarator_srt = resolve_declarator(resolver);
    declarator_srt->dtype = dtype_connect(declarator_srt->dtype, resolver->shared_dtype);
    vector_push(srt->children, declarator_srt);

    if (!symboltable_can_define(resolver->global_table, declarator_srt->ident_name)) {
        fprintf(stderr, "Error: identifier '%s' is already defined\n", declarator_srt->ident_name);
        exit(1);
    }
    char* table_ident_name = new_string(declarator_srt->ident_name);
    Dtype* table_dtype = dtype_copy(declarator_srt->dtype);
    symboltable_define(resolver->global_table, table_ident_name, table_dtype);

    resolver->local_table = new_symboltable();

    Vector* params = declarator_srt->dtype->function->params;
    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        DParam* dparam = vector_at(params, i);
        if (!symboltable_can_define(resolver->local_table, dparam->ident_name)) {
            fprintf(stderr, "Error: identifier '%s' is already defined\n", dparam->ident_name);
            exit(1);
        }
        char* table_ident_name = new_string(dparam->ident_name);
        Dtype* table_dtype = dtype_copy(dparam->dtype);
        symboltable_define(resolver->local_table, table_ident_name, table_dtype);
    }

    resolver->ast = vector_at(ast->children, 2);
    vector_push(srt->children, resolve_compound_stmt(resolver));

    delete_symboltable(resolver->local_table);
    resolver->local_table = NULL;

    resolver->shared_dtype = NULL;
    resolver->ast = ast;
    return srt;
}
