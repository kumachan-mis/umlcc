#include "./external.h"
#include "../common/common.h"
#include "./declaration.h"
#include "./statement.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_transration_unit(Resolver* resolver) {
    Srt* srt = new_srt(SRT_TRAS_UNIT, 0);
    Ast* ast = resolver->_ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        vector_push(srt->children, resolve_function_definition(resolver));
    }

    resolver->_ast = ast;
    return srt;
}

Srt* resolve_function_definition(Resolver* resolver) {
    Srt* srt = new_srt(SRT_FUNC_DEF, 0);
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    resolver->_shared_ctype = resolve_decl_specifiers(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* declarator_srt = resolve_declarator(resolver);
    declarator_srt->ctype = ctype_connect(declarator_srt->ctype, resolver->_shared_ctype);
    vector_push(srt->children, declarator_srt);

    if (!symboltable_can_define(resolver->_global_table, declarator_srt->ident_name)) {
        fprintf(stderr, "Error: identifier '%s' is already defined\n", declarator_srt->ident_name);
        exit(1);
    }
    char* table_ident_name = string_copy(declarator_srt->ident_name);
    CType* table_ctype = ctype_copy(declarator_srt->ctype);
    symboltable_define(resolver->_global_table, table_ident_name, table_ctype);

    resolver->_local_table = new_symboltable();

    Vector* params = declarator_srt->ctype->function->params;
    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        CParam* cparam = vector_at(params, i);
        if (!symboltable_can_define(resolver->_local_table, cparam->ident_name)) {
            fprintf(stderr, "Error: identifier '%s' is already defined\n",
                    declarator_srt->ident_name);
            exit(1);
        }
        table_ident_name = string_copy(cparam->ident_name);
        table_ctype = ctype_copy(cparam->ctype);
        symboltable_define(resolver->_local_table, table_ident_name, table_ctype);
    }

    resolver->_ast = vector_at(ast->children, 2);
    vector_push(srt->children, resolve_compound_stmt(resolver));

    delete_symboltable(resolver->_local_table);
    resolver->_local_table = NULL;

    resolver->_shared_ctype = NULL;
    resolver->_ast = ast;
    return srt;
}
