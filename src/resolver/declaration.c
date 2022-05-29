#include "./declaration.h"
#include "../common/common.h"
#include "./expression.h"

#include <stdio.h>
#include <stdlib.h>

Srt* resolve_decl(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolver->shared_dtype = resolve_decl_specifiers(resolver);

    resolver->ast = vector_at(ast->children, 1);
    srt = resolve_init_declarator_list(resolver);

    delete_dtype(resolver->shared_dtype);
    resolver->shared_dtype = NULL;
    resolver->ast = ast;
    return srt;
}

Dtype* resolve_decl_specifiers(Resolver* resolver) {
    Dtype* dtype = NULL;
    Ast* ast = resolver->ast;

    Ast* ast_ptr = vector_at(ast->children, 0);
    switch (ast_ptr->type) {
        case AST_TYPE_INT:
            dtype = new_integer_dtype();
            break;
        default:
            fprintf(stderr, "Error: unexpected ast type %d\n", ast_ptr->type);
            exit(1);
    }

    return dtype;
}

Srt* resolve_init_declarator_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_DECL_LIST, 0);
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->ast = vector_at(ast->children, i);
        vector_push(srt->children, resolve_init_declarator(resolver));
    }

    resolver->ast = ast;
    return srt;
}

Srt* resolve_init_declarator(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT_DECL, 0);
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* declarator = resolve_declarator(resolver);
    declarator->dtype = dtype_connect(declarator->dtype, dtype_copy(resolver->shared_dtype));

    char* symbol_name = new_string(declarator->ident_name);
    Dtype* symbol_dtype = dtype_copy(declarator->dtype);

    if (resolver->local_table == NULL) {
        SymbolTable* table = resolver->global_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else if (symbol_dtype->type == DTYPE_FUNCUCTION) {
        SymbolTable* table = resolver->local_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else {
        SymbolTable* table = resolver->local_table;
        symboltable_define_memory(table, symbol_name, symbol_dtype);
    }

    vector_push(srt->children, declarator);

    resolver->ast = ast;
    return srt;
}

Srt* resolve_declarator(Resolver* resolver) {
    Dtype* dtype = NULL;
    Ast* ast = resolver->ast;

    Ast* ast_ptr = ast;

    while (1) {
        switch (ast_ptr->type) {
            case AST_PTR_DECLOR: {
                Dtype* socket_dtype = new_socket_pointer_dtype();
                dtype = dtype_connect(socket_dtype, dtype);
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_ARRAY_DECLOR: {
                // TODO: support expression for array size
                resolver->ast = vector_at(ast_ptr->children, 1);
                Srt* array_size = resolve_expr(resolver);
                Dtype* socket_dtype = new_socket_array_dtype(array_size->value_int);
                delete_srt(array_size);
                dtype = dtype_connect(socket_dtype, dtype);
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_FUNC_DECLOR: {
                resolver->ast = vector_at(ast_ptr->children, 1);
                Dtype* socket_dtype = new_socket_function_dtype(resolve_parameter_list(resolver));
                dtype = dtype_connect(socket_dtype, dtype);
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_IDENT_DECLOR:
                resolver->ast = ast;
                return new_identifier_srt(SRT_DECL, dtype, new_string(ast_ptr->ident_name));
            default:
                fprintf(stderr, "Error: unexpected ast type %d\n", ast_ptr->type);
                exit(1);
        }
    }
}

Vector* resolve_parameter_list(Resolver* resolver) {
    Vector* params = new_vector(&t_dparam);
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        resolver->ast = vector_at(ast->children, i);
        vector_push(params, resolve_parameter_decl(resolver));
    }

    resolver->ast = ast;
    return params;
}

DParam* resolve_parameter_decl(Resolver* resolver) {
    DParam* dparam = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Dtype* specifiers_dtype = resolve_decl_specifiers(resolver);

    resolver->ast = vector_at(ast->children, 1);
    Srt* srt = resolve_declarator(resolver);
    srt->dtype = dtype_connect(srt->dtype, specifiers_dtype);

    dparam = new_dparam(new_string(srt->ident_name), dtype_copy(srt->dtype));
    delete_srt(srt);

    resolver->ast = ast;
    return dparam;
}
