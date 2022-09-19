#include "./declaration.h"
#include "../common/type.h"
#include "../literal/iliteral.h"
#include "../set/set.h"
#include "./conversion.h"
#include "./expression.h"

#include <stdio.h>
#include <stdlib.h>

ResolverReturn* resolve_decl(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&resolver->specifier_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;

    if (errs != NULL) return new_resolverret_errors(errs);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&srt, &errs, resolve_init_declarator_list(resolver));
    resolver->ast = ast;

    if (errs != NULL) return new_resolverret_errors(errs);

    delete_dtype(resolver->specifier_dtype);
    resolver->specifier_dtype = NULL;
    return new_resolverret(srt);
}

ResolverReturnDtype* resolve_decl_specifiers(Resolver* resolver) {
    Dtype* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    int i = 0, num_children = vector_size(ast->children);
    while (i < num_children) {
        Ast* child = vector_at(ast->children, i);
        switch (child->type) {
            case AST_STG_TYPEDEF:
                if (dtype == NULL) dtype = new_socket_decoration_dtype();
                dtype->decoration->typedef_flag = 1;
                vector_erase(ast->children, i);
                num_children--;
                break;
            default:
                i++;
                break;
        }
    }

    if (vector_size(ast->children) != 1) {
        errs = new_vector(&t_error);
        err = new_error("Error: declaration specifiers are invalid\n");
        vector_push(errs, err);
        return new_resolverret_dtype_errors(errs);
    }

    Ast* child = vector_at(ast->children, 0);
    switch (child->type) {
        case AST_TYPE_INT:
            dtype = dtype_connect(dtype, new_integer_dtype(DTYPE_INT));
            break;
        case AST_TYPE_CHAR:
            dtype = dtype_connect(dtype, new_integer_dtype(DTYPE_CHAR));
            break;
        case AST_TYPEDEF_NAME: {
            Symbol* symbol = NULL;
            if (symbol == NULL && resolver->local_table != NULL) {
                symbol = symboltable_search(resolver->local_table, child->ident_name);
            }
            if (symbol == NULL) symbol = symboltable_search(resolver->global_table, child->ident_name);
            dtype = dtype_connect(dtype, dtype_copy(symbol->dtype->decoration->deco_dtype));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: unreachable statement"
                            " (in resolve_decl_specifiers)\n");
            exit(1);
    }

    if (errs != NULL) {
        delete_dtype(dtype);
        return new_resolverret_dtype_errors(errs);
    }
    return new_resolverret_dtype(dtype);
}

ResolverReturn* resolve_init_declarator_list(Resolver* resolver) {
    Srt* srt = new_srt(SRT_DECL_LIST, 0);
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;
        resolver->ast = vector_at(ast->children, i);
        resolverret_assign(&child_srt, &child_errs, resolve_init_declarator(resolver));

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

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_init_declarator(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT_DECL, 0);
    Srt* child_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_assign(&child_srt, &errs, resolve_declarator(resolver));
    resolver->ast = ast;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    if (resolver->specifier_dtype->type == DTYPE_DECORATION) {
        Dtype* decoration_dtype = dtype_copy(resolver->specifier_dtype);
        Dtype* specifier_dtype = decoration_dtype->decoration->deco_dtype;
        decoration_dtype->decoration->deco_dtype = NULL;
        child_srt->dtype = dtype_connect(child_srt->dtype, specifier_dtype);
        child_srt->dtype = dtype_connect(decoration_dtype, child_srt->dtype);
    } else {
        Dtype* specifier_dtype = dtype_copy(resolver->specifier_dtype);
        child_srt->dtype = dtype_connect(child_srt->dtype, specifier_dtype);
    }

    SymbolTable* table = resolver->local_table;
    if (table == NULL) table = resolver->global_table;

    if (!symboltable_can_define(table, child_srt->ident_name)) {
        errs = new_vector(&t_error);
        err = new_error("Error: identifier '%s' is already declared\n", child_srt->ident_name);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    char* symbol_name = new_string(child_srt->ident_name);
    Dtype* symbol_dtype = dtype_copy(child_srt->dtype);
    if (resolver->local_table == NULL || !dtype_isobject(symbol_dtype)) {
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else {
        symboltable_define_memory(table, symbol_name, symbol_dtype);
    }

    if (vector_size(ast->children) == 1) return new_resolverret(srt);

    resolver->initialized_dtype = child_srt->dtype;
    resolver->initialized_offset = 0;
    resolver->is_nested_initializing = 0;
    resolver->ast = vector_at(ast->children, 1);

    resolverret_assign(&child_srt, &errs, resolve_initializer(resolver));

    resolver->ast = ast;
    resolver->is_nested_initializing = 0;
    resolver->initialized_offset = -1;
    resolver->initialized_dtype = NULL;

    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    vector_push(srt->children, child_srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_declarator(Resolver* resolver) {
    Srt* srt = NULL;
    Dtype* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Dtype* socket_dtype = NULL;
    Dtype* plug_dtype = NULL;
    Ast* ast = resolver->ast;
    Ast* ast_ptr = ast;

    while (srt == NULL && errs == NULL) {
        switch (ast_ptr->type) {
            case AST_PTR_DECLOR: {
                plug_dtype = new_socket_pointer_dtype();
                dtype = dtype_connect(socket_dtype, plug_dtype);
                socket_dtype = plug_dtype;
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_ARRAY_DECLOR: {
                if (socket_dtype != NULL && socket_dtype->type == DTYPE_FUNCTION) {
                    errs = new_vector(&t_error);
                    err = new_error("Error: function returning array is invalid\n");
                    vector_push(errs, err);
                    break;
                }

                Srt* array_size_srt = NULL;
                resolver->ast = vector_at(ast_ptr->children, 1);
                resolverret_assign(&array_size_srt, &errs, resolve_expr(resolver));
                if (errs != NULL) break;

                if (array_size_srt->type != SRT_INT_EXPR || array_size_srt->iliteral->is_unsigned) {
                    errs = new_vector(&t_error);
                    err = new_error("Error: only direct integer is supported as array size\n");
                    vector_push(errs, err);
                    delete_srt(array_size_srt);
                    break;
                }

                plug_dtype = new_socket_array_dtype(array_size_srt->iliteral->signed_value);
                delete_srt(array_size_srt);
                dtype = dtype_connect(socket_dtype, plug_dtype);
                socket_dtype = plug_dtype;
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_FUNC_DECLOR: {
                if (socket_dtype != NULL && socket_dtype->type == DTYPE_ARRAY) {
                    errs = new_vector(&t_error);
                    err = new_error("Error: array of functions is invalid\n");
                    vector_push(errs, err);
                    break;
                } else if (socket_dtype != NULL && socket_dtype->type == DTYPE_FUNCTION) {
                    errs = new_vector(&t_error);
                    err = new_error("Error: function returning function is invalid\n");
                    vector_push(errs, err);
                    break;
                }

                Vector* func_dparams = NULL;
                resolver->ast = vector_at(ast_ptr->children, 1);
                resolverret_dparams_assign(&func_dparams, &errs, resolve_parameter_list(resolver));
                if (errs != NULL) break;

                plug_dtype = new_socket_function_dtype(func_dparams);
                dtype = dtype_connect(socket_dtype, plug_dtype);
                socket_dtype = plug_dtype;
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_IDENT_DECLOR:
                srt = new_identifier_srt(SRT_DECL, dtype, new_string(ast_ptr->ident_name));
                break;
            default:
                fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: unreachable statement"
                                " (in resolve_declarator)\n");
                exit(1);
        }
    }

    resolver->ast = ast;
    if (errs != NULL) {
        if (dtype != NULL) delete_dtype(dtype);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturnDParams* resolve_parameter_list(Resolver* resolver) {
    Vector* dparams = new_vector(&t_dparam);
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;
    Set* param_names_set = new_set(&t_hashable_string);

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        DParam* dparam = NULL;
        Vector* child_errs = NULL;
        resolver->ast = vector_at(ast->children, i);

        resolverret_dparam_assign(&dparam, &child_errs, resolve_parameter_decl(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) errs = new_vector(&t_error);
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        }

        if (set_contains(param_names_set, dparam->ident_name)) {
            if (errs == NULL) errs = new_vector(&t_error);
            err = new_error("Error: parameter '%s' is already declared\n", dparam->ident_name);
            vector_push(errs, err);
        }
        if (errs != NULL) {
            delete_dparam(dparam);
            continue;
        }

        vector_push(dparams, dparam);
        set_add(param_names_set, new_string(dparam->ident_name));
    }

    resolver->ast = ast;
    delete_set(param_names_set);
    if (errs != NULL) {
        delete_vector(dparams);
        return new_resolverret_dparams_errors(errs);
    }
    return new_resolverret_dparams(dparams);
}

ResolverReturnDParam* resolve_parameter_decl(Resolver* resolver) {
    Dtype* specifiers_dtype = NULL;
    Srt* declarator_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&specifiers_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_dparam_errors(errs);

    if (specifiers_dtype->type == DTYPE_DECORATION && specifiers_dtype->decoration->typedef_flag) {
        errs = new_vector(&t_error);
        err = new_error("Error: storage specifiers are invalid for a function parameter\n");
        vector_push(errs, err);
        delete_dtype(specifiers_dtype);
        return new_resolverret_dparam_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&declarator_srt, &errs, resolve_declarator(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_dtype(specifiers_dtype);
        return new_resolverret_dparam_errors(errs);
    }

    declarator_srt->dtype = dtype_connect(declarator_srt->dtype, specifiers_dtype);
    if (declarator_srt->dtype->type == DTYPE_ARRAY) {
        Dtype* array_of_dtype = dtype_copy(declarator_srt->dtype->array->of_dtype);
        declarator_srt->dtype = new_pointer_dtype(array_of_dtype);
    }
    if (declarator_srt->dtype->type == DTYPE_FUNCTION) {
        Dtype* function_dtype = dtype_copy(declarator_srt->dtype);
        declarator_srt->dtype = new_pointer_dtype(function_dtype);
    }

    char* dparam_ident_name = new_string(declarator_srt->ident_name);
    Dtype* dparam_dtype = dtype_copy(declarator_srt->dtype);
    DParam* dparam = new_dparam(dparam_ident_name, dparam_dtype);
    delete_srt(declarator_srt);

    resolver->ast = ast;
    return new_resolverret_dparam(dparam);
}

ResolverReturn* resolve_initializer(Resolver* resolver) {
    ResolverReturn* resolve_array_initializer(Resolver * resolver);
    ResolverReturn* resolve_string_initializer(Resolver * resolver);
    ResolverReturn* resolve_scalar_initializer(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Dtype* dtype = resolver->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_CHAR:
        case DTYPE_INT:
        case DTYPE_POINTER:
            resolverret_assign(&srt, &errs, resolve_scalar_initializer(resolver));
            break;
        case DTYPE_ARRAY:
            if (dtype->array->of_dtype->type == DTYPE_CHAR) {
                resolverret_assign(&srt, &errs, resolve_string_initializer(resolver));
            } else {
                resolverret_assign(&srt, &errs, resolve_array_initializer(resolver));
            }
            break;
        case DTYPE_FUNCTION:
            errs = new_vector(&t_error);
            err = new_error("Error: function cannot be initialized\n");
            vector_push(errs, err);
            break;
        case DTYPE_DECORATION:
            errs = new_vector(&t_error);
            err = new_error("Error: typedef-name cannot be initialized\n");
            vector_push(errs, err);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: unreachable statement"
                            " (in resolve_initializer)\n");
            exit(1);
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverReturn* resolve_zero_initializer(Resolver* resolver) {
    ResolverReturn* resolve_zero_array_initializer(Resolver * resolver);
    ResolverReturn* resolve_zero_scalar_initializer(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;

    Dtype* dtype = resolver->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_CHAR:
        case DTYPE_INT:
        case DTYPE_POINTER:
            // resolve_zero_scalar_initializer does not return an error
            resolverret_assign(&srt, &errs, resolve_zero_scalar_initializer(resolver));
            break;
        case DTYPE_ARRAY:
            // resolve_zero_array_initializer does not return an error
            resolverret_assign(&srt, &errs, resolve_zero_array_initializer(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: unreachable statement"
                            " (in resolve_zero_initializer)\n");
            exit(1);
    }

    return new_resolverret(srt);
}

ResolverReturn* resolve_array_initializer(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT, 0);
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    Dtype* dtype = resolver->initialized_dtype;
    int initializer_len = vector_size(ast->children);

    if (ast->type != AST_INIT_LIST) {
        errs = new_vector(&t_error);
        err = new_error("Error: an array should be initialized with an initializer list\n");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (initializer_len == 0) {
        errs = new_vector(&t_error);
        err = new_error("Error: one or more initializer is required in an initializer list\n");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    int array_index = 0;

    while (resolver->initialized_offset < initializer_len && array_index < dtype->array->size) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        Ast* child_ast = vector_at(ast->children, resolver->initialized_offset);
        int original_offset = resolver->initialized_offset;
        int original_is_nested = resolver->is_nested_initializing;

        resolver->initialized_dtype = dtype->array->of_dtype;

        if (dtype_isaggregate(dtype->array->of_dtype) && child_ast->type != AST_INIT_LIST) {
            resolver->ast = ast;
            resolver->is_nested_initializing = 1;
            resolverret_assign(&child_srt, &child_errs, resolve_initializer(resolver));
        } else {
            resolver->ast = child_ast;
            resolver->initialized_offset = 0;
            resolver->is_nested_initializing = 0;
            resolverret_assign(&child_srt, &child_errs, resolve_initializer(resolver));
            resolver->initialized_offset = original_offset + 1;
        }
        resolver->is_nested_initializing = original_is_nested;

        array_index++;

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

    for (int i = array_index; i < dtype->array->size; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        resolver->ast = NULL;
        resolver->initialized_dtype = dtype->array->of_dtype;

        // resolve_zero_initializer does not return an error
        resolverret_assign(&child_srt, &child_errs, resolve_zero_initializer(resolver));
        vector_push(srt->children, child_srt);
    }

    if (!resolver->is_nested_initializing && array_index == dtype->array->size &&
        resolver->initialized_offset < initializer_len) {
        if (errs != NULL) delete_vector(errs);
        errs = new_vector(&t_error);
        err = new_error("Error: initializer list is too long\n");
        vector_push(errs, err);
    }

    resolver->ast = ast;
    resolver->initialized_dtype = dtype;
    if (errs != NULL) {
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturn* resolve_string_initializer(Resolver* resolver) {
    Srt* srt = NULL;
    Ast* literal_ast = resolver->ast;
    Dtype* dtype = resolver->initialized_dtype;

    if (literal_ast->type == AST_INIT_LIST && vector_size(literal_ast->children) == 1) {
        literal_ast = vector_at(literal_ast->children, 0);
    }
    if (literal_ast->type == AST_STRING_EXPR) {
        int size = dtype->array->size;
        StringLiteral* sliteral = sliteral_zero_padding_copy(literal_ast->sliteral, size);
        srt = new_sliteral_srt(SRT_STRING_EXPR, dtype_copy(dtype), sliteral);
        srt = new_srt(SRT_INIT, 1, srt);
        return new_resolverret(srt);
    }

    return resolve_array_initializer(resolver);
}

ResolverReturn* resolve_zero_array_initializer(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT, 0);

    Dtype* dtype = resolver->initialized_dtype;

    for (int i = 0; i < dtype->array->size; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;
        resolver->initialized_dtype = dtype->array->of_dtype;

        // resolve_zero_initializer does not return an error
        resolverret_assign(&child_srt, &child_errs, resolve_zero_initializer(resolver));
        vector_push(srt->children, child_srt);
    }

    resolver->initialized_dtype = dtype;
    return new_resolverret(srt);
}

ResolverReturn* resolve_scalar_initializer(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    Dtype* dtype = resolver->initialized_dtype;

    resolver->initialized_offset++;

    if (ast->type == AST_INIT_LIST) {
        int init_list_len = vector_size(ast->children);
        if (init_list_len == 0) {
            errs = new_vector(&t_error);
            err = new_error("Error: one or more initializer is required in an initializer list\n");
            vector_push(errs, err);
            return new_resolverret_errors(errs);
        }
        if (init_list_len > 1) {
            errs = new_vector(&t_error);
            err = new_error("Error: initializer list is too long\n");
            vector_push(errs, err);
            return new_resolverret_errors(errs);
        }
        resolver->ast = vector_at(ast->children, 0);
    }

    if (!ast_isexpr(resolver->ast)) {
        resolver->ast = ast;
        errs = new_vector(&t_error);
        err = new_error("Error: expression or that enclosed with braces is required\n");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;

    if (errs != NULL) return new_resolverret_errors(errs);

    srt = convert_to_ptr_if_array(srt);
    srt = convert_to_ptr_if_function(srt);

    if (!dtype_isassignable(dtype, srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("Error: expression is not assignable to declared object\n");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    if (!dtype_equals(dtype, srt->dtype)) srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);
    srt = new_srt(SRT_INIT, 1, srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_zero_scalar_initializer(Resolver* resolver) {
    Srt* srt = NULL;
    Dtype* dtype = resolver->initialized_dtype;

    IntegerLiteral* zero_iliteral = new_signed_iliteral(INTEGER_INT, 0);
    srt = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), zero_iliteral);
    if (!dtype_equals(dtype, srt->dtype)) srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);

    srt = new_srt(SRT_INIT, 1, srt);
    return new_resolverret(srt);
}
