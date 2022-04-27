#include "./declaration.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>


Vector* resolve_decl(Resolver* resolver) {
    Vector* srts = NULL;
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    resolver->_decl_specifiers_ctype = resolve_decl_specifiers(resolver);

    resolver->_ast = vector_at(ast->children, 1);
    srts = resolve_init_declarator_list(resolver);

    delete_ctype(resolver->_decl_specifiers_ctype);
    resolver->_decl_specifiers_ctype = NULL;
    resolver->_ast = ast;
    return srts;
}

CType* resolve_decl_specifiers(Resolver* resolver) {
    CType* ctype = NULL;

    Ast* ast = resolver->_ast;
    Ast* specifier_header_ast = vector_at(ast->children, 0);
    switch (specifier_header_ast->type) {
        case AST_TYPE_INT:
            ctype = new_integer_ctype();
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", specifier_header_ast->type);
            exit(1);
    }

    return ctype;
}

Vector* resolve_init_declarator_list(Resolver* resolver) {
    Vector* srts = new_vector();
    Ast* ast = resolver->_ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        vector_push(srts, resolve_init_declarator(resolver));
    }

    resolver->_ast = ast;
    return srts;
}

Srt* resolve_init_declarator(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    srt = resolve_declarator(resolver);
    resolver->_ast = ast;

    return srt;
}

Srt* resolve_declarator(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    srt = resolve_direct_declarator(resolver);
    srt->ctype = ctype_connect(srt->ctype, ctype_copy(resolver->_decl_specifiers_ctype));
    resolver->_ast = ast;

    if (!symboltable_can_define(resolver->_local_table, srt->ident_name)) {
        fprintf(stderr, "Error: identifier '%s' is already defined\n", srt->ident_name);
        exit(1);
    }
    char* table_ident_name = string_copy(srt->ident_name);
    CType* table_ctype = ctype_copy(srt->ctype);
    symboltable_define(resolver->_local_table, table_ident_name, table_ctype);

    return srt;
}

Srt* resolve_direct_declarator(Resolver* resolver) {
    Srt* srt = NULL;
    CType* ctype = NULL;

    Ast* ast = resolver->_ast;

    int terminated = 0;
    while (!terminated) {
        switch (ast->type) {
            case AST_FUNC_DECLOR: {
                CType* socket_ctype = new_socket_function_ctype(new_vector());
                ctype_connect(socket_ctype, ctype);
                ctype = socket_ctype;
                ast = vector_at(ast->children, 0);
                break;
            }
            case AST_IDENT_DECLOR:
                srt = new_identifier_srt(SRT_OBJ_DECL, ctype, string_copy(ast->ident_name));
                terminated = 1;
                break;
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        } 
    }
}
