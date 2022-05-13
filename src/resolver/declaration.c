#include "./declaration.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_decl(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    resolver->_shared_ctype = resolve_decl_specifiers(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    srt = resolve_init_declarator_list(resolver);

    delete_ctype(resolver->_shared_ctype);
    resolver->_shared_ctype = NULL;
    resolver->_ast = ast;
    return srt;
}

CType* resolve_decl_specifiers(Resolver* resolver) {
    CType* ctype = NULL;
    Ast* ast = resolver->_ast;

    Ast* lookedup_ast = vector_at(ast->children, 0);
    switch (lookedup_ast->type) {
        case AST_TYPE_INT:
            ctype = new_integer_ctype();
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", lookedup_ast->type);
            exit(1);
    }

    return ctype;
}

Srt* resolve_init_declarator_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_DECL_LIST, 0);
    Ast* ast = resolver->_ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        vector_push(srt->children, resolve_init_declarator(resolver));
    }

    resolver->_ast = ast;
    return srt;
}

Srt* resolve_init_declarator(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT_DECL, 0);
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* declarator_srt = resolve_declarator(resolver);
    declarator_srt->ctype =
        ctype_connect(declarator_srt->ctype, ctype_copy(resolver->_shared_ctype));

    SymbolTable* table = resolver->_global_table;
    if (resolver->_local_table != NULL) table = resolver->_local_table;

    if (!symboltable_can_define(table, declarator_srt->ident_name)) {
        fprintf(stderr, "Error: identifier '%s' is already defined\n", declarator_srt->ident_name);
        exit(1);
    }
    char* table_ident_name = new_string(declarator_srt->ident_name);
    CType* table_ctype = ctype_copy(declarator_srt->ctype);
    symboltable_define(table, table_ident_name, table_ctype);

    vector_push(srt->children, declarator_srt);

    resolver->_ast = ast;
    return srt;
}

Srt* resolve_declarator(Resolver* resolver) {
    Srt* srt = NULL;
    CType* ctype = NULL;
    Ast* ast = resolver->_ast;

    Ast* lookedup_ast = ast;
    CType* socket_ctype = NULL;
    int terminated = 0;

    while (!terminated) {
        switch (lookedup_ast->type) {
            case AST_FUNC_DECLOR:
                resolver->_ast = vector_at(lookedup_ast->children, 1);
                socket_ctype = new_socket_function_ctype(resolve_parameter_list(resolver));
                ctype_connect(socket_ctype, ctype);
                ctype = socket_ctype;
                lookedup_ast = vector_at(lookedup_ast->children, 0);
                break;
            case AST_IDENT_DECLOR:
                srt = new_identifier_srt(SRT_DECL, ctype, new_string(lookedup_ast->ident_name));
                terminated = 1;
                break;
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", lookedup_ast->type);
                exit(1);
        }
    }

    resolver->_ast = ast;
    return srt;
}

Vector* resolve_parameter_list(Resolver* resolver) {
    Vector* params = new_vector();
    Ast* ast = resolver->_ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        vector_push(params, resolve_parameter_decl(resolver));
    }

    resolver->_ast = ast;
    return params;
}

CParam* resolve_parameter_decl(Resolver* resolver) {
    CParam* cparam = NULL;
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    CType* specifiers_ctype = resolve_decl_specifiers(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    Srt* srt = resolve_declarator(resolver);
    srt->ctype = ctype_connect(srt->ctype, specifiers_ctype);

    cparam = new_cparam(new_string(srt->ident_name), ctype_copy(srt->ctype));
    delete_srt(srt);

    resolver->_ast = ast;
    return cparam;
}
