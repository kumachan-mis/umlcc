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

    if (vector_size(ast->children) == 1) {
        srt = new_srt(SRT_DECL_LIST, 0);
        delete_dtype(resolver->specifier_dtype);
        resolver->specifier_dtype = NULL;
        return new_resolverret(srt);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&srt, &errs, resolve_init_declarator_list(resolver));
    resolver->ast = ast;

    delete_dtype(resolver->specifier_dtype);
    resolver->specifier_dtype = NULL;

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverDTypeReturn* resolve_decl_specifiers(Resolver* resolver) {
    ResolverDTypeReturn* resolve_type_specifier_list(Resolver * resolver);

    DType* dtype = NULL;
    DType* specifiers_dtype = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;

    int i = 0, num_children = vector_size(ast->children);
    while (i < num_children) {
        Ast* child = vector_at(ast->children, i);
        switch (child->type) {
            case AST_STG_TYPEDEF:
                if (dtype == NULL) dtype = new_socket_decoration_dtype();
                dtype->ddecoration->typedef_flag = 1;
                vector_erase(ast->children, i);
                num_children--;
                break;
            default:
                i++;
                break;
        }
    }

    resolverret_dtype_assign(&specifiers_dtype, &errs, resolve_type_specifier_list(resolver));

    if (errs != NULL) return new_resolverret_dtype_errors(errs);

    dtype = dtype_connect(dtype, specifiers_dtype);
    return new_resolverret_dtype(dtype);
}

ResolverDTypeReturn* resolve_specifier_qualifier_list(Resolver* resolver) {
    ResolverDTypeReturn* resolve_type_specifier_list(Resolver * resolver);

    return resolve_type_specifier_list(resolver);
}

ResolverDTypeReturn* resolve_type_specifier_list(Resolver* resolver) {
    DType* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    if (vector_size(ast->children) != 1) {
        errs = new_vector(&t_error);
        err = new_error("combination of type specifiers is invalid\n");
        vector_push(errs, err);
        return new_resolverret_dtype_errors(errs);
    }

    Ast* child = vector_at(ast->children, 0);
    switch (child->type) {
        case AST_TYPE_INT:
            dtype = new_integer_dtype(DTYPE_INT);
            break;
        case AST_TYPE_CHAR:
            dtype = new_integer_dtype(DTYPE_CHAR);
            break;
        case AST_TYPE_STRUCT:
            resolver->ast = child;
            resolverret_dtype_assign(&dtype, &errs, resolve_struct_specifier(resolver));
            resolver->ast = ast;
            break;
        case AST_TYPEDEF_NAME: {
            Symbol* symbol = symboltable_search(resolver->symbol_table, child->ident_name);
            dtype = dtype_copy(symbol->dtype->ddecoration->deco_dtype);
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_type_specifier_list)\n");
            exit(1);
    }

    if (errs != NULL) return new_resolverret_dtype_errors(errs);
    return new_resolverret_dtype(dtype);
}

ResolverDTypeReturn* resolve_struct_specifier(Resolver* resolver) {
    DType* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    Ast* child = NULL;

    char* struct_name = NULL;
    child = vector_at(ast->children, 0);
    if (child != NULL && child->type == AST_STRUCT_NAME) struct_name = new_string(child->ident_name);

    Vector* members = NULL;
    child = vector_at(ast->children, struct_name != NULL ? 1 : 0);
    if (child != NULL && child->type == AST_STRUCT_DECL_LIST) {
        resolver->ast = child;
        resolverret_dmembers_assign(&members, &errs, resolve_struct_decl_list(resolver));
        resolver->ast = ast;
    }
    if (errs != NULL) {
        if (struct_name != NULL) free(struct_name);
        return new_resolverret_dtype_errors(errs);
    }

    if (struct_name == NULL) {
        dtype = new_unnamed_struct_dtype(members);
        return new_resolverret_dtype(dtype);
    }

    if (members == NULL) {
        DType* unnamed_dtype = tagtable_search_struct(resolver->tag_table, struct_name);
        int nbytes = unnamed_dtype != NULL ? unnamed_dtype->dstruct->nbytes : 0;
        dtype = new_named_struct_dtype(struct_name, nbytes);
        return new_resolverret_dtype(dtype);
    }

    if (!tagtable_can_define_struct(resolver->tag_table, struct_name)) {
        errs = new_vector(&t_error);
        err = new_error("struct '%s' is already declared\n", struct_name);
        vector_push(errs, err);
        free(struct_name);
        delete_vector(members);
        return new_resolverret_dtype_errors(errs);
    }

    tagtable_define_struct(resolver->tag_table, new_string(struct_name), members);

    DType* unnamed_dtype = tagtable_search_struct(resolver->tag_table, struct_name);
    int nbytes = unnamed_dtype != NULL ? unnamed_dtype->dstruct->nbytes : 0;
    dtype = new_named_struct_dtype(struct_name, nbytes);
    return new_resolverret_dtype(dtype);
}

ResolverReturnDMembers* resolve_struct_decl_list(Resolver* resolver) {
    Vector* members = new_vector(&t_dmember);
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        Vector* child_members = NULL;
        Vector* child_errs = NULL;

        resolver->ast = vector_at(ast->children, i);
        resolverret_dmembers_assign(&child_members, &child_errs, resolve_struct_decl(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) errs = new_vector(&t_error);
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        } else if (errs != NULL) {
            delete_vector(child_members);
            continue;
        }

        vector_extend(members, child_members);
        delete_vector(child_members);
    }

    resolver->ast = ast;

    if (errs != NULL) {
        delete_vector(members);
        return new_resolverret_dmembers_errors(errs);
    }

    Set* member_names_set = new_set(&t_hashable_string);
    int num_members = vector_size(members);
    for (int i = 0; i < num_members; i++) {
        DMember* member = vector_at(members, i);
        if (!set_contains(member_names_set, member->name)) {
            set_add(member_names_set, new_string(member->name));
            continue;
        }
        if (errs == NULL) errs = new_vector(&t_error);
        err = new_error("struct member '%s' is already declared\n", member->name);
        vector_push(errs, err);
    }
    delete_set(member_names_set);

    if (errs != NULL) {
        delete_vector(members);
        return new_resolverret_dmembers_errors(errs);
    }
    return new_resolverret_dmembers(members);
}

ResolverReturnDMembers* resolve_struct_decl(Resolver* resolver) {
    Vector* members = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&resolver->specifier_dtype, &errs, resolve_specifier_qualifier_list(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_dmembers_errors(errs);

    resolver->ast = vector_at(ast->children, 1);
    resolverret_dmembers_assign(&members, &errs, resolve_struct_declarator_list(resolver));
    resolver->ast = ast;

    delete_dtype(resolver->specifier_dtype);
    resolver->specifier_dtype = NULL;

    if (errs != NULL) return new_resolverret_dmembers_errors(errs);
    return new_resolverret_dmembers(members);
}

ResolverReturnDMembers* resolve_struct_declarator_list(Resolver* resolver) {
    Vector* members = new_vector(&t_dmember);
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        DMember* member = NULL;
        Vector* child_errs = NULL;
        resolver->ast = vector_at(ast->children, i);
        resolverret_dmember_assign(&member, &child_errs, resolve_struct_declarator(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) errs = new_vector(&t_error);
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        } else if (errs != NULL) {
            delete_dmember(member);
            continue;
        }

        vector_push(members, member);
    }

    resolver->ast = ast;

    if (errs != NULL) {
        delete_vector(members);
        return new_resolverret_dmembers_errors(errs);
    }
    return new_resolverret_dmembers(members);
}

ResolverReturnDMember* resolve_struct_declarator(Resolver* resolver) {
    DMember* member = NULL;
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    resolverret_assign(&srt, &errs, resolve_declarator(resolver));
    if (errs != NULL) return new_resolverret_dmember_errors(errs);

    DType* specifier_dtype = dtype_copy(resolver->specifier_dtype);
    srt->dtype = dtype_connect(srt->dtype, specifier_dtype);

    if (srt->dtype->type == DTYPE_FUNCTION || dtype_isincomplete(srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("struct member should not have incomplete or function type\n");
        vector_push(errs, err);
        return new_resolverret_dmember_errors(errs);
    }

    member = new_dmember(new_string(srt->ident_name), dtype_copy(srt->dtype));
    delete_srt(srt);
    return new_resolverret_dmember(member);
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
    // TODO: validate incomplete type declaration

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
        DType* decoration_dtype = dtype_copy(resolver->specifier_dtype);
        DType* specifier_dtype = decoration_dtype->ddecoration->deco_dtype;
        decoration_dtype->ddecoration->deco_dtype = NULL;
        child_srt->dtype = dtype_connect(child_srt->dtype, specifier_dtype);
        child_srt->dtype = dtype_connect(decoration_dtype, child_srt->dtype);
    } else {
        DType* specifier_dtype = dtype_copy(resolver->specifier_dtype);
        child_srt->dtype = dtype_connect(child_srt->dtype, specifier_dtype);
    }

    if (!symboltable_can_define(resolver->symbol_table, child_srt->ident_name)) {
        errs = new_vector(&t_error);
        err = new_error("identifier '%s' is already declared\n", child_srt->ident_name);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    char* symbol_name = new_string(child_srt->ident_name);
    DType* symbol_dtype = dtype_copy(child_srt->dtype);
    if (resolver->symbol_table->outer_scope == NULL || !dtype_isobject(symbol_dtype)) {
        symboltable_define_label(resolver->symbol_table, symbol_name, symbol_dtype);
    } else {
        symboltable_define_memory(resolver->symbol_table, symbol_name, symbol_dtype);
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
    DType* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    DType* socket_dtype = NULL;
    DType* plug_dtype = NULL;
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
                    err = new_error("function returning array is invalid\n");
                    vector_push(errs, err);
                    break;
                }

                Srt* array_size_srt = NULL;
                resolver->ast = vector_at(ast_ptr->children, 1);
                resolverret_assign(&array_size_srt, &errs, resolve_expr(resolver));
                if (errs != NULL) break;

                if (array_size_srt->type != SRT_INT_EXPR || array_size_srt->iliteral->is_unsigned) {
                    errs = new_vector(&t_error);
                    err = new_error("only direct integer is supported as array size\n");
                    vector_push(errs, err);
                    delete_srt(array_size_srt);
                    break;
                }

                if (array_size_srt->iliteral->signed_value <= 0) {
                    errs = new_vector(&t_error);
                    err = new_error("array size should be a positive integer\n");
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
                    err = new_error("array of functions is invalid\n");
                    vector_push(errs, err);
                    break;
                } else if (socket_dtype != NULL && socket_dtype->type == DTYPE_FUNCTION) {
                    errs = new_vector(&t_error);
                    err = new_error("function returning function is invalid\n");
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

        if (set_contains(param_names_set, dparam->name)) {
            if (errs == NULL) errs = new_vector(&t_error);
            err = new_error("parameter '%s' is already declared\n", dparam->name);
            vector_push(errs, err);
        }
        if (errs != NULL) {
            delete_dparam(dparam);
            continue;
        }

        vector_push(dparams, dparam);
        set_add(param_names_set, new_string(dparam->name));
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
    DType* specifiers_dtype = NULL;
    Srt* declarator_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&specifiers_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;
    if (errs != NULL) return new_resolverret_dparam_errors(errs);

    if (specifiers_dtype->type == DTYPE_DECORATION && specifiers_dtype->ddecoration->typedef_flag) {
        errs = new_vector(&t_error);
        err = new_error("storage specifiers are invalid for a function parameter\n");
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
        DType* array_of_dtype = dtype_copy(declarator_srt->dtype->darray->of_dtype);
        delete_dtype(declarator_srt->dtype);
        declarator_srt->dtype = new_pointer_dtype(array_of_dtype);
    }
    if (declarator_srt->dtype->type == DTYPE_FUNCTION) {
        DType* function_dtype = dtype_copy(declarator_srt->dtype);
        delete_dtype(declarator_srt->dtype);
        declarator_srt->dtype = new_pointer_dtype(function_dtype);
    }

    char* dparam_ident_name = new_string(declarator_srt->ident_name);
    DType* dparam_dtype = dtype_copy(declarator_srt->dtype);
    DParam* dparam = new_dparam(dparam_ident_name, dparam_dtype);

    delete_srt(declarator_srt);

    return new_resolverret_dparam(dparam);
}

ResolverReturn* resolve_initializer(Resolver* resolver) {
    ResolverReturn* resolve_scalar_initializer(Resolver * resolver);
    ResolverReturn* resolve_aggregate_initializer(Resolver * resolver);
    ResolverReturn* resolve_string_initializer(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    DType* dtype = resolver->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_CHAR:
        case DTYPE_INT:
        case DTYPE_POINTER:
            resolverret_assign(&srt, &errs, resolve_scalar_initializer(resolver));
            break;
        case DTYPE_ARRAY:
            if (dtype->darray->of_dtype->type == DTYPE_CHAR) {
                resolverret_assign(&srt, &errs, resolve_string_initializer(resolver));
                break;
            }
            resolverret_assign(&srt, &errs, resolve_aggregate_initializer(resolver));
            break;
        case DTYPE_STRUCT:
            if (dtype->dstruct->members == NULL) {
                resolver->initialized_dtype = tagtable_search_struct(resolver->tag_table, dtype->dstruct->name);
            }
            resolverret_assign(&srt, &errs, resolve_aggregate_initializer(resolver));
            break;
        case DTYPE_FUNCTION:
            errs = new_vector(&t_error);
            err = new_error("function cannot be initialized\n");
            vector_push(errs, err);
            break;
        case DTYPE_DECORATION:
            errs = new_vector(&t_error);
            err = new_error("typedef-name cannot be initialized\n");
            vector_push(errs, err);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_initializer)\n");
            exit(1);
    }

    if (errs != NULL) return new_resolverret_errors(errs);
    return new_resolverret(srt);
}

ResolverReturn* resolve_zero_initializer(Resolver* resolver) {
    ResolverReturn* resolve_zero_scalar_initializer(Resolver * resolver);
    ResolverReturn* resolve_zero_aggregate_initializer(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;

    DType* dtype = resolver->initialized_dtype;

    switch (dtype->type) {
        case DTYPE_CHAR:
        case DTYPE_INT:
        case DTYPE_POINTER:
            // resolve_zero_scalar_initializer does not return an error
            resolverret_assign(&srt, &errs, resolve_zero_scalar_initializer(resolver));
            break;
        case DTYPE_ARRAY:
        case DTYPE_STRUCT:
            // resolve_zero_aggregate_initializer does not return an error
            resolverret_assign(&srt, &errs, resolve_zero_aggregate_initializer(resolver));
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_zero_initializer)\n");
            exit(1);
    }

    return new_resolverret(srt);
}

ResolverReturn* resolve_scalar_initializer(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    DType* dtype = resolver->initialized_dtype;

    resolver->initialized_offset++;

    if (ast->type == AST_INIT_LIST) {
        int init_list_len = vector_size(ast->children);
        if (init_list_len > 1) {
            errs = new_vector(&t_error);
            err = new_error("initializer list is too long\n");
            vector_push(errs, err);
            return new_resolverret_errors(errs);
        }
        resolver->ast = vector_at(ast->children, 0);
    }

    if (!ast_isexpr(resolver->ast)) {
        resolver->ast = ast;
        errs = new_vector(&t_error);
        err = new_error("expression or that enclosed with braces is required\n");
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
        err = new_error("expression is not assignable to declared object\n");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_equals(dtype, srt->dtype)) srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);
    srt = new_srt(SRT_INIT, 1, srt);

    return new_resolverret(srt);
}

ResolverReturn* resolve_aggregate_initializer(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT, 0);
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    DType* dtype = resolver->initialized_dtype;
    int initializer_len = vector_size(ast->children);

    if (ast->type != AST_INIT_LIST) {
        errs = new_vector(&t_error);
        err = new_error("initializer list is required\n");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    int aggregate_size = dtype_aggregate_size(dtype), aggregate_index = 0;
    while (resolver->initialized_offset < initializer_len && aggregate_index < aggregate_size) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        Ast* child_ast = vector_at(ast->children, resolver->initialized_offset);
        int original_offset = resolver->initialized_offset;
        int original_is_nested = resolver->is_nested_initializing;

        resolver->initialized_dtype = dtype_aggregate_at(dtype, aggregate_index);

        if (dtype_isaggregate(resolver->initialized_dtype) && child_ast->type != AST_INIT_LIST) {
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

        aggregate_index++;

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

    for (int i = aggregate_index; i < aggregate_size; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        resolver->ast = NULL;
        resolver->initialized_dtype = dtype_aggregate_at(dtype, aggregate_index);

        // resolve_zero_initializer does not return an error
        resolverret_assign(&child_srt, &child_errs, resolve_zero_initializer(resolver));
        vector_push(srt->children, child_srt);
    }

    if (!resolver->is_nested_initializing && aggregate_index == aggregate_size &&
        resolver->initialized_offset < initializer_len) {
        if (errs != NULL) delete_vector(errs);
        errs = new_vector(&t_error);
        err = new_error("initializer list is too long\n");
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
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* literal_ast = resolver->ast;
    DType* dtype = resolver->initialized_dtype;

    if (literal_ast->type == AST_INIT_LIST && vector_size(literal_ast->children) == 1) {
        literal_ast = vector_at(literal_ast->children, 0);
    }

    if (literal_ast->type != AST_STRING_EXPR) return resolve_aggregate_initializer(resolver);

    if (literal_ast->sliteral->size - 1 > dtype->darray->size) {
        errs = new_vector(&t_error);
        err = new_error("initializer string literal is too long\n");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    StringLiteral* sliteral = sliteral_zero_padding_copy(literal_ast->sliteral, dtype->darray->size);
    srt = new_sliteral_srt(SRT_STRING_EXPR, dtype_copy(dtype), sliteral);
    srt = new_srt(SRT_INIT, 1, srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_zero_scalar_initializer(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = resolver->initialized_dtype;

    IntegerLiteral* zero_iliteral = new_signed_iliteral(INTEGER_INT, 0);
    srt = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), zero_iliteral);
    if (!dtype_equals(dtype, srt->dtype)) srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);

    srt = new_srt(SRT_INIT, 1, srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_zero_aggregate_initializer(Resolver* resolver) {
    Srt* srt = new_srt(SRT_INIT, 0);

    DType* dtype = resolver->initialized_dtype;
    int aggregate_size = dtype_aggregate_size(dtype);

    for (int i = 0; i < aggregate_size; i++) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;
        resolver->initialized_dtype = dtype_aggregate_at(dtype, i);

        // resolve_zero_initializer does not return an error
        resolverret_assign(&child_srt, &child_errs, resolve_zero_initializer(resolver));
        vector_push(srt->children, child_srt);
    }

    resolver->initialized_dtype = dtype;
    return new_resolverret(srt);
}
