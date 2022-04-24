#include "./external.h"
#include "./declaration.h"
#include "./statement.h"
#include "../common/common.h"

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
    CType* specifiers_ctype = resolve_decl_specifiers(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* decl_srt = resolve_declarator(resolver);
    decl_srt->ctype = ctype_connect(decl_srt->ctype, specifiers_ctype);
    vector_push(srt->children, decl_srt);

    if (!symboltable_can_define(resolver->_global_table, decl_srt->ident_name)) {
        fprintf(stderr, "Error: identifier '%s' is already defined\n", decl_srt->ident_name);
        exit(1);
    }
    symboltable_define(resolver->_global_table, string_copy(decl_srt->ident_name), ctype_copy(decl_srt->ctype));

    resolver->_local_table = new_symboltable();
    resolver->_ast = vector_at(ast->children, 2);
    vector_push(srt->children, resolve_compound_stmt(resolver));
    delete_symboltable(resolver->_local_table);
    resolver->_local_table = NULL;

    resolver->_ast = ast;
    return srt;
}
