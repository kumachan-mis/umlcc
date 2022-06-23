#include "./declaration.h"
#include "../common/type.h"
#include "./conversion.h"
#include "./expression.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Srt* resolve_decl(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolver->specifier_dtype = resolve_decl_specifiers(resolver);

    resolver->ast = vector_at(ast->children, 1);
    srt = resolve_init_declarator_list(resolver);

    delete_dtype(resolver->specifier_dtype);
    resolver->specifier_dtype = NULL;
    resolver->ast = ast;
    return srt;
}

Dtype* resolve_decl_specifiers(Resolver* resolver) {
    Dtype* dtype = NULL;
    Ast* ast = resolver->ast;

    Ast* ast_ptr = vector_at(ast->children, 0);
    switch (ast_ptr->type) {
        case AST_TYPE_INT:
            dtype = new_integer_dtype(DTYPE_INT);
            break;
        case AST_TYPE_CHAR:
            dtype = new_integer_dtype(DTYPE_CHAR);
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
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    Srt* declarator_srt = resolve_declarator(resolver);
    Dtype* specifier_dtype = dtype_copy(resolver->specifier_dtype);
    declarator_srt->dtype = dtype_connect(declarator_srt->dtype, specifier_dtype);

    char* symbol_name = new_string(declarator_srt->ident_name);
    Dtype* symbol_dtype = dtype_copy(declarator_srt->dtype);

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

    if (vector_size(ast->children) == 1) {
        resolver->ast = ast;
        return new_srt(SRT_INIT_DECL, 1, declarator_srt);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolver->initialized_dtype = declarator_srt->dtype;
    resolver->initialized_offset = 0;

    Srt* initializer_srt = resolve_initializer(resolver);

    resolver->ast = ast;
    resolver->initialized_offset = -1;
    resolver->initialized_dtype = NULL;
    return new_srt(SRT_INIT_DECL, 2, declarator_srt, initializer_srt);
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
                Srt* array_size_srt = resolve_expr(resolver);
                Dtype* socket_dtype = new_socket_array_dtype(array_size_srt->value_int);
                delete_srt(array_size_srt);
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
    if (srt->dtype->type == DTYPE_ARRAY) {
        srt->dtype = new_pointer_dtype(dtype_copy(srt->dtype->array->of_dtype));
    }
    if (srt->dtype->type == DTYPE_FUNCUCTION) {
        srt->dtype = new_pointer_dtype(dtype_copy(srt->dtype));
    }

    dparam = new_dparam(new_string(srt->ident_name), dtype_copy(srt->dtype));
    delete_srt(srt);

    resolver->ast = ast;
    return dparam;
}

Srt* resolve_initializer(Resolver* resolver) {
    Srt* resolve_array_initializer(Resolver * resolver);
    Srt* resolve_string_initializer(Resolver * resolver);
    Srt* resolve_scalar_initializer(Resolver * resolver);

    Dtype* dtype = resolver->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_ARRAY:
            if (dtype->array->of_dtype->type == DTYPE_CHAR) {
                return resolve_string_initializer(resolver);
            }
            return resolve_array_initializer(resolver);
        default:
            if (dtype_isscalar(dtype)) return resolve_scalar_initializer(resolver);
            break;
    }

    fprintf(stderr, "Error: unexpected data type %d\n", dtype->type);
    exit(1);
}

Srt* resolve_zero_initializer(Resolver* resolver) {
    Srt* resolve_zero_array_initializer(Resolver * resolver);
    Srt* resolve_zero_scalar_initializer(Resolver * resolver);

    Dtype* dtype = resolver->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_ARRAY:
            return resolve_zero_array_initializer(resolver);
        default:
            if (dtype_isscalar(dtype)) return resolve_zero_scalar_initializer(resolver);
            break;
    }

    fprintf(stderr, "Error: unexpected data type %d\n", dtype->type);
    exit(1);
}

Srt* resolve_array_initializer(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT, 0);
    Ast* ast = resolver->ast;
    Dtype* dtype = resolver->initialized_dtype;

    int initializer_len = vector_size(ast->children);
    int array_index = 0;

    while (resolver->initialized_offset < initializer_len && array_index < dtype->array->size) {
        Ast* child_ast = vector_at(ast->children, resolver->initialized_offset);
        resolver->initialized_dtype = dtype->array->of_dtype;

        if (dtype_isaggregate(dtype->array->of_dtype) && child_ast->type != AST_INIT_LIST) {
            resolver->ast = ast;
            vector_push(srt->children, resolve_initializer(resolver));
            array_index++;
            continue;
        }

        int offset = resolver->initialized_offset;
        resolver->ast = child_ast;
        resolver->initialized_offset = 0;
        vector_push(srt->children, resolve_initializer(resolver));
        resolver->initialized_offset = offset + 1;
        array_index++;
    }

    for (int i = array_index; i < dtype->array->size; i++) {
        resolver->ast = NULL;
        resolver->initialized_dtype = dtype->array->of_dtype;
        vector_push(srt->children, resolve_zero_initializer(resolver));
    }

    resolver->ast = ast;
    resolver->initialized_dtype = dtype;
    return srt;
}

Srt* resolve_string_initializer(Resolver* resolver) {
    Dtype* dtype = resolver->initialized_dtype;

    Ast* literal_ast = resolver->ast;
    if (literal_ast->type == AST_INIT_LIST) literal_ast = vector_at(literal_ast->children, 0);
    if (literal_ast->type == AST_STRING_EXPR) {
        int size = dtype->array->size;
        StringLiteral* sliteral = sliteral_zero_padding_copy(literal_ast->sliteral, size);
        Srt* srt = new_sliteral_srt(SRT_STRING_EXPR, dtype_copy(dtype), sliteral);
        return new_srt(SRT_INIT, 1, srt);
    }

    return resolve_array_initializer(resolver);
}

Srt* resolve_zero_array_initializer(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT, 0);
    Dtype* dtype = resolver->initialized_dtype;

    for (int i = 0; i < dtype->array->size; i++) {
        resolver->initialized_dtype = dtype->array->of_dtype;
        vector_push(srt->children, resolve_zero_initializer(resolver));
    }

    resolver->initialized_dtype = dtype;
    return srt;
}

Srt* resolve_scalar_initializer(Resolver* resolver) {
    Ast* ast = resolver->ast;
    Dtype* dtype = resolver->initialized_dtype;

    if (ast->type == AST_INIT_LIST) resolver->ast = vector_at(ast->children, 0);

    Srt* srt = resolve_expr(resolver);
    srt = convert_to_ptr_if_array(srt);
    srt = convert_to_ptr_if_function(srt);
    if (!dtype_equals(dtype, srt->dtype)) {
        srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);
    }
    resolver->initialized_offset++;

    resolver->ast = ast;
    return new_srt(SRT_INIT, 1, srt);
}

Srt* resolve_zero_scalar_initializer(Resolver* resolver) {
    Dtype* dtype = resolver->initialized_dtype;

    Srt* srt = new_integer_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), 0);
    if (!dtype_equals(dtype, srt->dtype)) {
        srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);
    }

    return new_srt(SRT_INIT, 1, srt);
}
