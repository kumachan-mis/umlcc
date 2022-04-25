#include "./declaration.h"
#include "../common/common.h"

#include <stdio.h>
#include <stdlib.h>


Vector* resolve_decl(Resolver* resolver) {
    Vector* srts = new_vector();
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    CType* specifiers_ctype = resolve_decl_specifiers(resolver);

    int num_children = vector_size(ast->children);
    for (int i = 1; i < num_children; i++) {
        resolver->_ast = vector_at(ast->children, i);
        Vector* init_declor_srts = resolve_init_declarator(resolver);
    
        Srt* decl_srt = vector_at(init_declor_srts, 0);
        decl_srt->ctype = ctype_connect(decl_srt->ctype, ctype_copy(specifiers_ctype));

        if (!symboltable_can_define(resolver->_local_table, decl_srt->ident_name)) {
            fprintf(stderr, "Error: identifier '%s' is already defined\n", decl_srt->ident_name);
            exit(1);
        }
        symboltable_define(resolver->_local_table, string_copy(decl_srt->ident_name), ctype_copy(decl_srt->ctype));

        vector_extend(srts, init_declor_srts);
        delete_vector(init_declor_srts, (void (*)(void* item))delete_srt);
    }

    delete_ctype(specifiers_ctype);
    resolver->_ast = ast;
    return srts;
}

CType* resolve_decl_specifiers(Resolver* resolver) {
    CType* ctype = NULL;
    Ast* ast = resolver->_ast;
    
    Ast* child = vector_at(ast->children, 0);
    switch (child->type) {
        case AST_TYPE_INT:
            ctype = new_integer_ctype();
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", child->type);
            exit(1);

    }

    return ctype;
}

Vector* resolve_init_declarator(Resolver* resolver) {
    Vector* srts = new_vector();
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    vector_push(srts, resolve_declarator(resolver));

    resolver->_ast = ast;
    return srts;
}

Srt* resolve_declarator(Resolver* resolver) {
    Ast* ast = resolver->_ast;

    resolver->_ast = vector_at(ast->children, 0);
    Srt* srt = resolve_direct_declarator(resolver);

    resolver->_ast = ast;
    return srt;
}

Srt* resolve_direct_declarator(Resolver* resolver) {
    Ast* ast = resolver->_ast;
    CType* ctype = NULL;

    while (1) {
        switch (ast->type) {
            case AST_FUNC_DECLOR: {
                CType* socket_ctype = new_socket_function_ctype(new_vector());
                ctype_connect(socket_ctype, ctype);
                ctype = socket_ctype;
                ast = vector_at(ast->children, 0);
                break;
            }
            case AST_IDENT_DECLOR:
                return new_identifier_srt(SRT_OBJ_DECL, ctype, string_copy(ast->ident_name));
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast->type);
                exit(1);
        } 
    }
}
