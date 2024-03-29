#include "./declaration.h"
#include "../common/type.h"
#include "../error/errint.h"
#include "../literal/iliteral.h"
#include "../set/set.h"
#include "./conversion.h"
#include "./expression.h"
#include "./utils.h"

#include <stdio.h>
#include <stdlib.h>

ResolverReturn* resolve_decl(Resolver* resolver) {
    Srt* srt = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;
    DType* original_specifier_dtype = resolver->specifier_dtype;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&resolver->specifier_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_errors(errs);
    }

    if (vector_size(ast->children) == 1) {
        srt = new_srt(SRT_DECL_LIST, 0);
        delete_dtype(resolver->specifier_dtype);
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret(srt);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&srt, &errs, resolve_init_declarator_list(resolver));
    resolver->ast = ast;

    delete_dtype(resolver->specifier_dtype);
    resolver->specifier_dtype = original_specifier_dtype;

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
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
        Ast* child_ast = vector_at(ast->children, i);
        switch (child_ast->type) {
            case AST_STG_TYPEDEF:
                if (dtype == NULL) {
                    dtype = new_socket_typedef_dtype();
                }
                vector_erase(ast->children, i);
                num_children--;
                break;
            default:
                i++;
                break;
        }
    }

    resolverret_dtype_assign(&specifiers_dtype, &errs, resolve_type_specifier_list(resolver));

    if (errs != NULL) {
        if (dtype != NULL) {
            delete_dtype(dtype);
        }
        return new_resolverret_dtype_errors(errs);
    }

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

    int num_type_specifiers = vector_size(ast->children);
    if (num_type_specifiers < 1 || num_type_specifiers > 4) {
        // The longest type specifiers are "signed long long int" and "unsigned long long int"
        errs = new_vector(&t_error);
        err = new_error("combination of type specifiers is invalid");
        vector_push(errs, err);
        return new_resolverret_dtype_errors(errs);
    }

    AstType* type_specifiers = malloc(sizeof(AstType) * num_type_specifiers);
    for (int i = 0; i < num_type_specifiers; i++) {
        Ast* child = vector_at(ast->children, i);
        type_specifiers[i] = child->type;
    }
    sort_ast_type(type_specifiers, 0, num_type_specifiers - 1);

    if (
        // void
        num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_VOID) {
        dtype = new_void_dtype();
    } else if (
        // char
        num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_CHAR) {
        dtype = new_integer_dtype(DTYPE_CHAR);
    } else if (
        // int
        num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_INT) {
        dtype = new_integer_dtype(DTYPE_INT);
    } else if (
        // long
        (num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_LONG) ||
        // long int
        (num_type_specifiers == 2 && type_specifiers[0] == AST_TYPE_INT && type_specifiers[1] == AST_TYPE_LONG)) {
        dtype = new_integer_dtype(DTYPE_LONG);
    } else if (
        // long long
        (num_type_specifiers == 2 && type_specifiers[0] == AST_TYPE_LONG && type_specifiers[1] == AST_TYPE_LONG) ||
        // long long int
        (num_type_specifiers == 3 && type_specifiers[0] == AST_TYPE_INT && type_specifiers[1] == AST_TYPE_LONG &&
         type_specifiers[2] == AST_TYPE_LONG)) {
        dtype = new_integer_dtype(DTYPE_LONGLONG);
    } else if (
        // unsigned
        (num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_UNSIGNED) ||
        // unsigned int
        (num_type_specifiers == 2 && type_specifiers[0] == AST_TYPE_INT && type_specifiers[1] == AST_TYPE_UNSIGNED)) {
        dtype = new_integer_dtype(DTYPE_UNSIGNED_INT);
    } else if (
        // unsigned long
        (num_type_specifiers == 2 && type_specifiers[0] == AST_TYPE_LONG && type_specifiers[1] == AST_TYPE_UNSIGNED) ||
        // unsigned long int
        (num_type_specifiers == 3 && type_specifiers[0] == AST_TYPE_INT && type_specifiers[1] == AST_TYPE_LONG &&
         type_specifiers[2] == AST_TYPE_UNSIGNED)) {
        dtype = new_integer_dtype(DTYPE_UNSIGNED_LONG);
    } else if (
        // unsigned long long
        (num_type_specifiers == 3 && type_specifiers[0] == AST_TYPE_LONG && type_specifiers[1] == AST_TYPE_LONG &&
         type_specifiers[2] == AST_TYPE_UNSIGNED) ||
        // unsigned long long int
        (num_type_specifiers == 4 && type_specifiers[0] == AST_TYPE_INT && type_specifiers[1] == AST_TYPE_LONG &&
         type_specifiers[2] == AST_TYPE_LONG && type_specifiers[3] == AST_TYPE_UNSIGNED)) {
        dtype = new_integer_dtype(DTYPE_UNSIGNED_LONGLONG);
    } else if (
        // struct
        num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_STRUCT) {
        resolver->ast = vector_at(ast->children, 0);
        resolverret_dtype_assign(&dtype, &errs, resolve_struct_specifier(resolver));
        resolver->ast = ast;
    } else if (
        // enum
        num_type_specifiers == 1 && type_specifiers[0] == AST_TYPE_ENUM) {
        resolver->ast = vector_at(ast->children, 0);
        resolverret_dtype_assign(&dtype, &errs, resolve_enum_specifier(resolver));
        resolver->ast = ast;
    } else if (
        // typedef-name
        num_type_specifiers == 1 && type_specifiers[0] == AST_TYPEDEF_NAME) {
        resolver->ast = vector_at(ast->children, 0);
        resolverret_dtype_assign(&dtype, &errs, resolve_typedef_name_specifier(resolver));
        resolver->ast = ast;
    } else {
        errs = new_vector(&t_error);
        err = new_error("combination of type specifiers is invalid");
        vector_push(errs, err);
    }

    free(type_specifiers);
    if (errs != NULL) {
        return new_resolverret_dtype_errors(errs);
    }
    return new_resolverret_dtype(dtype);
}

ResolverDTypeReturn* resolve_struct_specifier(Resolver* resolver) {
    DType* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    Ast* child_ast = NULL;

    char* struct_name = NULL;
    child_ast = vector_at(ast->children, 0);
    if (child_ast != NULL && child_ast->type == AST_STRUCT_NAME) {
        struct_name = new_string(child_ast->ident_name);
    }

    Vector* members = NULL;
    child_ast = vector_at(ast->children, struct_name != NULL ? 1 : 0);
    if (child_ast != NULL && child_ast->type == AST_STRUCT_DECL_LIST) {
        resolver->ast = child_ast;
        resolverret_dstructmembers_assign(&members, &errs, resolve_struct_decl_list(resolver));
        resolver->ast = ast;
    }
    if (errs != NULL) {
        if (struct_name != NULL) {
            free(struct_name);
        }
        return new_resolverret_dtype_errors(errs);
    }

    if (struct_name == NULL) {
        dtype = new_unnamed_struct_dtype(members);
        return new_resolverret_dtype(dtype);
    }

    if (members == NULL) {
        DType* unnamed_dtype = tagtable_search(resolver->tag_table, struct_name);
        int nbytes = 0, alignment = 0;
        if (unnamed_dtype != NULL) {
            nbytes = unnamed_dtype->dstruct->nbytes;
            alignment = unnamed_dtype->dstruct->alignment;
        }
        dtype = new_named_struct_dtype(struct_name, nbytes, alignment);
        return new_resolverret_dtype(dtype);
    }

    if (!tagtable_can_define(resolver->tag_table, struct_name)) {
        errs = new_vector(&t_error);
        err = new_error("tag '%s' is already declared", struct_name);
        vector_push(errs, err);
        free(struct_name);
        delete_vector(members);
        return new_resolverret_dtype_errors(errs);
    }

    tagtable_define_struct(resolver->tag_table, new_string(struct_name), members);

    DType* unnamed_dtype = tagtable_search(resolver->tag_table, struct_name);
    Srt* tag_decl_srt = new_identifier_srt(SRT_TAG_DECL, dtype_copy(unnamed_dtype), new_string(struct_name));
    vector_push(resolver->scope_srt->children, tag_decl_srt);

    dtype = new_named_struct_dtype(struct_name, unnamed_dtype->dstruct->nbytes, unnamed_dtype->dstruct->alignment);
    return new_resolverret_dtype(dtype);
}

ResolverReturnDStructMembers* resolve_struct_decl_list(Resolver* resolver) {
    Vector* members = new_vector(&t_dstructmember);
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        Vector* child_members = NULL;
        Vector* child_errs = NULL;

        resolver->ast = vector_at(ast->children, i);
        resolverret_dstructmembers_assign(&child_members, &child_errs, resolve_struct_decl(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
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
        return new_resolverret_dstructmembers_errors(errs);
    }

    Set* member_names_set = new_set(&t_hashable_string);
    int num_members = vector_size(members);
    for (int i = 0; i < num_members; i++) {
        DStructMember* member = vector_at(members, i);
        if (!set_contains(member_names_set, member->name)) {
            set_add(member_names_set, new_string(member->name));
            continue;
        }
        if (errs == NULL) {
            errs = new_vector(&t_error);
        }
        err = new_error("struct member '%s' is already declared", member->name);
        vector_push(errs, err);
    }
    delete_set(member_names_set);

    if (errs != NULL) {
        delete_vector(members);
        return new_resolverret_dstructmembers_errors(errs);
    }
    return new_resolverret_dstructmembers(members);
}

ResolverReturnDStructMembers* resolve_struct_decl(Resolver* resolver) {
    Vector* members = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;
    DType* original_specifier_dtype = resolver->specifier_dtype;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&resolver->specifier_dtype, &errs, resolve_specifier_qualifier_list(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_dstructmembers_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_dstructmembers_assign(&members, &errs, resolve_struct_declarator_list(resolver));
    resolver->ast = ast;

    delete_dtype(resolver->specifier_dtype);
    resolver->specifier_dtype = original_specifier_dtype;

    if (errs != NULL) {
        return new_resolverret_dstructmembers_errors(errs);
    }
    return new_resolverret_dstructmembers(members);
}

ResolverReturnDStructMembers* resolve_struct_declarator_list(Resolver* resolver) {
    Vector* members = new_vector(&t_dstructmember);
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        DStructMember* member = NULL;
        Vector* child_errs = NULL;
        resolver->ast = vector_at(ast->children, i);
        resolverret_dstructmember_assign(&member, &child_errs, resolve_struct_declarator(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        } else if (errs != NULL) {
            delete_dstructmember(member);
            continue;
        }

        vector_push(members, member);
    }

    resolver->ast = ast;

    if (errs != NULL) {
        delete_vector(members);
        return new_resolverret_dstructmembers_errors(errs);
    }
    return new_resolverret_dstructmembers(members);
}

ResolverReturnDStructMember* resolve_struct_declarator(Resolver* resolver) {
    DStructMember* member = NULL;
    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    resolverret_assign(&srt, &errs, resolve_declarator(resolver));
    if (errs != NULL) {
        return new_resolverret_dstructmember_errors(errs);
    }

    DType* specifier_dtype = dtype_copy(resolver->specifier_dtype);
    srt->dtype = dtype_connect(srt->dtype, specifier_dtype);

    if (dtype_isincomplete(srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("struct member '%s' has incomplete type", srt->ident_name);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_dstructmember_errors(errs);
    }

    if (srt->dtype->type == DTYPE_FUNCTION) {
        errs = new_vector(&t_error);
        err = new_error("struct member '%s' has function type", srt->ident_name);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_dstructmember_errors(errs);
    }

    member = new_dstructmember(new_string(srt->ident_name), dtype_copy(srt->dtype));
    delete_srt(srt);
    return new_resolverret_dstructmember(member);
}

ResolverDTypeReturn* resolve_enum_specifier(Resolver* resolver) {
    DType* dtype = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    Ast* child_ast = NULL;

    char* enum_name = NULL;
    child_ast = vector_at(ast->children, 0);
    if (child_ast != NULL && child_ast->type == AST_ENUM_NAME) {
        enum_name = new_string(child_ast->ident_name);
    }

    Vector* members = NULL;
    child_ast = vector_at(ast->children, enum_name != NULL ? 1 : 0);
    if (child_ast != NULL && child_ast->type == AST_ENUMOR_LIST) {
        resolver->ast = child_ast;
        resolverret_denummembers_assign(&members, &errs, resolve_enumerator_list(resolver));
        resolver->ast = ast;
    }
    if (errs != NULL) {
        if (enum_name != NULL) {
            free(enum_name);
        }
        return new_resolverret_dtype_errors(errs);
    }

    if (enum_name == NULL) {
        delete_vector(members);
        dtype = new_integer_dtype(DTYPE_INT);
        return new_resolverret_dtype(dtype);
    }

    if (members == NULL) {
        free(enum_name);
        dtype = new_integer_dtype(DTYPE_INT);
        return new_resolverret_dtype(dtype);
    }

    if (!tagtable_can_define(resolver->tag_table, enum_name)) {
        errs = new_vector(&t_error);
        err = new_error("tag '%s' is already declared", enum_name);
        vector_push(errs, err);
        free(enum_name);
        delete_vector(members);
        return new_resolverret_dtype_errors(errs);
    }

    tagtable_define_enum(resolver->tag_table, enum_name, members);

    dtype = new_integer_dtype(DTYPE_INT);
    return new_resolverret_dtype(dtype);
}

ResolverReturnDEnumMembers* resolve_enumerator_list(Resolver* resolver) {
    Vector* members = new_vector(&t_denummember);
    Vector* errs = NULL;
    Ast* ast = resolver->ast;

    resolver->default_enum_value = 0;
    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        DEnumMember* member = NULL;
        Vector* child_errs = NULL;
        resolver->ast = vector_at(ast->children, i);
        resolverret_denummember_assign(&member, &child_errs, resolve_enumerator(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        } else if (errs != NULL) {
            delete_denummember(member);
            continue;
        }

        vector_push(members, member);
    }

    resolver->ast = ast;
    resolver->default_enum_value = 0;

    if (errs != NULL) {
        delete_vector(members);
        return new_resolverret_denummembers_errors(errs);
    }
    return new_resolverret_denummembers(members);
}

ResolverReturnDEnumMember* resolve_enumerator(Resolver* resolver) {
    DEnumMember* member = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;

    Ast* child_ast = vector_at(ast->children, 0);
    if (!symboltable_can_define(resolver->symbol_table, child_ast->ident_name)) {
        errs = new_vector(&t_error);
        err = new_error("identifier '%s' is already declared", child_ast->ident_name);
        vector_push(errs, err);
        return new_resolverret_denummember_errors(errs);
    }

    if (vector_size(ast->children) == 1) {
        char* member_name = new_string(child_ast->ident_name);
        DType* member_dtype = new_integer_dtype(DTYPE_INT);
        IntegerLiteral* member_iliteral = new_signed_iliteral(INTEGER_INT, resolver->default_enum_value);
        symboltable_define_integer(resolver->symbol_table, member_name, member_dtype, member_iliteral);

        member = new_denummember(new_string(child_ast->ident_name), resolver->default_enum_value);
        resolver->default_enum_value++;

        return new_resolverret_denummember(member);
    }

    Srt* enum_const_srt = NULL;
    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&enum_const_srt, &errs, resolve_expr(resolver));
    if (errs != NULL) {
        return new_resolverret_denummember_errors(errs);
    }

    enum_const_srt = convert_to_ptr_if_array(enum_const_srt);
    enum_const_srt = convert_to_ptr_if_function(enum_const_srt);

    if (enum_const_srt->type != SRT_ILITERAL_EXPR || iliteral_isunsigned(enum_const_srt->iliteral)) {
        errs = new_vector(&t_error);
        err = new_error("only integer constant is supported as enumeration constant");
        vector_push(errs, err);
        delete_srt(enum_const_srt);
        return new_resolverret_denummember_errors(errs);
    }

    int enum_value = enum_const_srt->iliteral->signed_value;
    delete_srt(enum_const_srt);

    char* member_name = new_string(child_ast->ident_name);
    DType* member_dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* member_iliteral = new_signed_iliteral(INTEGER_INT, enum_value);
    symboltable_define_integer(resolver->symbol_table, member_name, member_dtype, member_iliteral);

    member = new_denummember(new_string(child_ast->ident_name), enum_value);
    resolver->default_enum_value = enum_value + 1;

    return new_resolverret_denummember(member);
}

ResolverDTypeReturn* resolve_typedef_name_specifier(Resolver* resolver) {
    DType* dtype = NULL;

    Ast* ast = resolver->ast;
    Symbol* symbol = symboltable_search(resolver->symbol_table, ast->ident_name);

    dtype = dtype_copy(symbol->dtype->dtypedef->defined_dtype);
    if (!dtype_isincomplete(dtype)) {
        return new_resolverret_dtype(dtype);
    }

    DType* complete_dtype = NULL;
    switch (dtype->type) {
        case DTYPE_STRUCT:
            complete_dtype = tagtable_search(resolver->tag_table, dtype->dstruct->name);
            if (complete_dtype == NULL)
                break;

            if (complete_dtype->type == DTYPE_STRUCT) {
                dtype->dstruct->nbytes = complete_dtype->dstruct->nbytes;
                dtype->dstruct->alignment = complete_dtype->dstruct->alignment;
                break;
            }
            // fall through
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in resolve_typedef_name_specifier)\n");
            exit(1);
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
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
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

    if (resolver->specifier_dtype->type == DTYPE_TYPEDEF) {
        DType* typedef_dtype = dtype_copy(resolver->specifier_dtype);
        DType* specifier_dtype = typedef_dtype->dtypedef->defined_dtype;
        typedef_dtype->dtypedef->defined_dtype = NULL;
        child_srt->dtype = dtype_connect(child_srt->dtype, specifier_dtype);
        child_srt->dtype = dtype_connect(typedef_dtype, child_srt->dtype);
    } else {
        DType* specifier_dtype = dtype_copy(resolver->specifier_dtype);
        child_srt->dtype = dtype_connect(child_srt->dtype, specifier_dtype);
    }

    if (!symboltable_can_define(resolver->symbol_table, child_srt->ident_name)) {
        errs = new_vector(&t_error);
        err = new_error("identifier '%s' is already declared", child_srt->ident_name);
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (dtype_isincomplete(child_srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("identifier '%s' has an incomplete type", child_srt->ident_name);
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

    if (vector_size(ast->children) == 1) {
        return new_resolverret(srt);
    }

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
    Ast* ast = resolver->ast;
    Ast* ast_ptr = ast;

    while (srt == NULL && errs == NULL) {
        switch (ast_ptr->type) {
            case AST_PTR_DECLOR: {
                DType* plug_dtype = new_socket_pointer_dtype();
                if (dtype == NULL) {
                    dtype = plug_dtype;
                }
                dtype_connect(socket_dtype, plug_dtype);
                socket_dtype = plug_dtype;
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_ARRAY_DECLOR: {
                if (socket_dtype != NULL && socket_dtype->type == DTYPE_FUNCTION) {
                    errs = new_vector(&t_error);
                    err = new_error("function returning an array is invalid");
                    vector_push(errs, err);
                    break;
                }

                Srt* array_size_srt = NULL;
                resolver->ast = vector_at(ast_ptr->children, 1);
                resolverret_assign(&array_size_srt, &errs, resolve_expr(resolver));
                if (errs != NULL) {
                    break;
                }

                array_size_srt = convert_to_ptr_if_array(array_size_srt);
                array_size_srt = convert_to_ptr_if_function(array_size_srt);

                if (array_size_srt->type != SRT_ILITERAL_EXPR || iliteral_isunsigned(array_size_srt->iliteral)) {
                    errs = new_vector(&t_error);
                    err = new_error("only signed integer constant is supported as array size");
                    vector_push(errs, err);
                    delete_srt(array_size_srt);
                    break;
                }

                if (array_size_srt->iliteral->signed_value <= 0) {
                    errs = new_vector(&t_error);
                    err = new_error("array size should be a positive integer");
                    vector_push(errs, err);
                    delete_srt(array_size_srt);
                    break;
                }

                DType* plug_dtype = new_socket_array_dtype(array_size_srt->iliteral->signed_value);
                delete_srt(array_size_srt);
                if (dtype == NULL) {
                    dtype = plug_dtype;
                }
                dtype_connect(socket_dtype, plug_dtype);
                socket_dtype = plug_dtype;
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_FUNC_DECLOR: {
                if (socket_dtype != NULL && socket_dtype->type == DTYPE_ARRAY) {
                    errs = new_vector(&t_error);
                    err = new_error("array of functions is invalid");
                    vector_push(errs, err);
                    break;
                }

                if (socket_dtype != NULL && socket_dtype->type == DTYPE_FUNCTION) {
                    errs = new_vector(&t_error);
                    err = new_error("function returning a function is invalid");
                    vector_push(errs, err);
                    break;
                }

                Vector* func_dparams = NULL;
                resolver->ast = vector_at(ast_ptr->children, 1);
                resolverret_dparams_assign(&func_dparams, &errs, resolve_parameter_list(resolver));
                if (errs != NULL) {
                    break;
                }

                DType* plug_dtype = new_socket_function_dtype(func_dparams);
                if (socket_dtype == NULL) {
                    dtype = plug_dtype;
                }
                dtype_connect(socket_dtype, plug_dtype);
                socket_dtype = plug_dtype;
                ast_ptr = vector_at(ast_ptr->children, 0);
                break;
            }
            case AST_IDENT_DECLOR:
            case AST_ABS_DECLOR: {
                DType* plug_dtype = resolver->specifier_dtype;
                if (plug_dtype->type == DTYPE_TYPEDEF) {
                    plug_dtype = plug_dtype->dtypedef->defined_dtype;
                }

                if (socket_dtype != NULL && socket_dtype->type == DTYPE_ARRAY && dtype_isincomplete(plug_dtype)) {
                    errs = new_vector(&t_error);
                    err = new_error("array of incomplete type is invalid");
                    vector_push(errs, err);
                    break;
                }

                if (socket_dtype != NULL && socket_dtype->type == DTYPE_FUNCTION && dtype_isincomplete(plug_dtype) &&
                    resolver->specifier_dtype->type != DTYPE_VOID) {
                    errs = new_vector(&t_error);
                    err = new_error("function returning incomplete type other than void is invalid");
                    vector_push(errs, err);
                    break;
                }

                if (ast_ptr->type == AST_IDENT_DECLOR) {
                    srt = new_identifier_srt(SRT_IDENT_DECL, dtype, new_string(ast_ptr->ident_name));
                } else {
                    srt = new_dtyped_srt(SRT_IDENT_DECL, dtype, 0);
                }
                break;
            }
            default:
                fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: unreachable statement"
                                " (in resolve_declarator)\n");
                exit(1);
        }
    }

    resolver->ast = ast;
    if (errs != NULL) {
        if (dtype != NULL) {
            delete_dtype(dtype);
        }
        return new_resolverret_errors(errs);
    }
    return new_resolverret(srt);
}

ResolverReturnDParams* resolve_parameter_list(Resolver* resolver) {
    Vector* dparams = new_vector(&t_dparam);
    Vector* errs = NULL;
    Error* err = NULL;
    Ast* ast = resolver->ast;

    if (ast_is_void_parameter_list(ast)) {
        return new_resolverret_dparams(dparams);
    }

    Set* param_names_set = new_set(&t_hashable_string);

    int num_children = vector_size(ast->children);
    for (int i = 0; i < num_children; i++) {
        DParam* dparam = NULL;
        Vector* child_errs = NULL;
        resolver->ast = vector_at(ast->children, i);

        resolverret_dparam_assign(&dparam, &child_errs, resolve_parameter_decl(resolver));

        if (child_errs != NULL) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        }

        if (dparam->name == NULL) {
            vector_push(dparams, dparam);
            continue;
        }

        if (set_contains(param_names_set, dparam->name)) {
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            err = new_error("function parameter '%s' is already declared", dparam->name);
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
    Srt* declarator_srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    Ast* ast = resolver->ast;
    DType* original_specifier_dtype = resolver->specifier_dtype;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&resolver->specifier_dtype, &errs, resolve_decl_specifiers(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_dparam_errors(errs);
    }

    if (resolver->specifier_dtype->type == DTYPE_TYPEDEF) {
        errs = new_vector(&t_error);
        err = new_error("storage specifiers are invalid for a function parameter");
        vector_push(errs, err);
        delete_dtype(resolver->specifier_dtype);
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_dparam_errors(errs);
    }

    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&declarator_srt, &errs, resolve_declarator(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_dtype(resolver->specifier_dtype);
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_dparam_errors(errs);
    }

    declarator_srt->dtype = dtype_connect(declarator_srt->dtype, resolver->specifier_dtype);
    resolver->specifier_dtype = original_specifier_dtype;

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

    if (dtype_isincomplete(declarator_srt->dtype)) {
        errs = new_vector(&t_error);
        if (declarator_srt->ident_name == NULL) {
            err = new_error("unnamed function parameter has an incomplete type");
        } else {
            err = new_error("function parameter '%s' has an incomplete type", declarator_srt->ident_name);
        }
        vector_push(errs, err);
        delete_srt(declarator_srt);
        return new_resolverret_dparam_errors(errs);
    }

    DType* dparam_dtype = dtype_copy(declarator_srt->dtype);
    DParam* dparam = NULL;
    if (declarator_srt->ident_name == NULL) {
        dparam = new_unnamed_dparam(dparam_dtype);
    } else {
        dparam = new_named_dparam(new_string(declarator_srt->ident_name), dparam_dtype);
    }

    delete_srt(declarator_srt);
    return new_resolverret_dparam(dparam);
}

ResolverDTypeReturn* resolve_type_name(Resolver* resolver) {
    DType* dtype = NULL;
    Vector* errs = NULL;

    Ast* ast = resolver->ast;
    DType* original_specifier_dtype = resolver->specifier_dtype;

    resolver->ast = vector_at(ast->children, 0);
    resolverret_dtype_assign(&resolver->specifier_dtype, &errs, resolve_specifier_qualifier_list(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_dtype_errors(errs);
    }

    Srt* child_srt = NULL;
    resolver->ast = vector_at(ast->children, 1);
    resolverret_assign(&child_srt, &errs, resolve_declarator(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        delete_dtype(resolver->specifier_dtype);
        resolver->specifier_dtype = original_specifier_dtype;
        return new_resolverret_dtype_errors(errs);
    }

    child_srt->dtype = dtype_connect(child_srt->dtype, resolver->specifier_dtype);
    dtype = dtype_copy(child_srt->dtype);
    delete_srt(child_srt);
    resolver->specifier_dtype = original_specifier_dtype;

    return new_resolverret_dtype(dtype);
}

ResolverReturn* resolve_initializer(Resolver* resolver) {
    ResolverReturn* resolve_scalar_initializer(Resolver * resolver);
    ResolverReturn* resolve_aggregate_initializer(Resolver * resolver);
    ResolverReturn* resolve_string_initializer(Resolver * resolver);

    Srt* srt = NULL;
    Vector* errs = NULL;
    Error* err = NULL;

    DType* dtype = resolver->initialized_dtype;

    if (dtype_isscalar(dtype)) {
        resolverret_assign(&srt, &errs, resolve_scalar_initializer(resolver));
    } else if (dtype->type == DTYPE_ARRAY && dtype->darray->of_dtype->type == DTYPE_CHAR) {
        resolverret_assign(&srt, &errs, resolve_string_initializer(resolver));
    } else if (dtype->type == DTYPE_STRUCT && dtype->dstruct->members == NULL) {
        resolver->initialized_dtype = tagtable_search(resolver->tag_table, dtype->dstruct->name);
        resolverret_assign(&srt, &errs, resolve_aggregate_initializer(resolver));
    } else if (dtype->type == DTYPE_ARRAY || dtype->type == DTYPE_STRUCT) {
        resolverret_assign(&srt, &errs, resolve_aggregate_initializer(resolver));
    } else if (dtype->type == DTYPE_FUNCTION) {
        errs = new_vector(&t_error);
        err = new_error("function cannot be initialized");
        vector_push(errs, err);
    } else if (dtype->type == DTYPE_TYPEDEF) {
        errs = new_vector(&t_error);
        err = new_error("typedef-name cannot be initialized");
        vector_push(errs, err);
    } else {
        fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                        "unreachable statement (in resolve_initializer)\n");
        exit(1);
    }

    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }
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
            // resolve_zero_aggregate_initializer does not return an error
            resolverret_assign(&srt, &errs, resolve_zero_aggregate_initializer(resolver));
            break;
        case DTYPE_STRUCT:
            if (dtype->dstruct->members == NULL) {
                resolver->initialized_dtype = tagtable_search(resolver->tag_table, dtype->dstruct->name);
            }
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
            err = new_error("initializer list is too long");
            vector_push(errs, err);
            return new_resolverret_errors(errs);
        }
        resolver->ast = vector_at(ast->children, 0);
    }

    if (!ast_isexpr(resolver->ast)) {
        resolver->ast = ast;
        errs = new_vector(&t_error);
        err = new_error("expression or that enclosed with braces is required");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    resolverret_assign(&srt, &errs, resolve_expr(resolver));
    resolver->ast = ast;
    if (errs != NULL) {
        return new_resolverret_errors(errs);
    }

    srt = convert_to_ptr_if_array(srt);
    srt = convert_to_ptr_if_function(srt);

    if (!dtype_isassignable(dtype, srt->dtype)) {
        errs = new_vector(&t_error);
        err = new_error("expression is not assignable to declared object");
        vector_push(errs, err);
        delete_srt(srt);
        return new_resolverret_errors(errs);
    }

    if (!dtype_equals(dtype, srt->dtype)) {
        srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);
    }
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
        err = new_error("initializer list is required");
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
            if (errs == NULL) {
                errs = new_vector(&t_error);
            }
            vector_extend(errs, child_errs);
            delete_vector(child_errs);
            continue;
        } else if (errs != NULL) {
            delete_srt(child_srt);
            continue;
        }

        vector_push(srt->children, child_srt);
    }

    while (aggregate_index < aggregate_size) {
        Srt* child_srt = NULL;
        Vector* child_errs = NULL;

        resolver->ast = NULL;
        resolver->initialized_dtype = dtype_aggregate_at(dtype, aggregate_index);

        // resolve_zero_initializer does not return an error
        resolverret_assign(&child_srt, &child_errs, resolve_zero_initializer(resolver));

        aggregate_index++;

        vector_push(srt->children, child_srt);
    }

    if (!resolver->is_nested_initializing && aggregate_index == aggregate_size &&
        resolver->initialized_offset < initializer_len) {
        if (errs != NULL) {
            delete_vector(errs);
        }
        errs = new_vector(&t_error);
        err = new_error("initializer list is too long");
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

    if (literal_ast->type != AST_STRING_EXPR) {
        return resolve_aggregate_initializer(resolver);
    }

    if (literal_ast->sliteral->size - 1 > dtype->darray->size) {
        errs = new_vector(&t_error);
        err = new_error("initializer string literal is too long");
        vector_push(errs, err);
        return new_resolverret_errors(errs);
    }

    StringLiteral* sliteral = sliteral_zero_padding_copy(literal_ast->sliteral, dtype->darray->size);
    srt = new_sliteral_srt(dtype_copy(dtype), sliteral);
    srt = new_srt(SRT_INIT, 1, srt);
    return new_resolverret(srt);
}

ResolverReturn* resolve_zero_scalar_initializer(Resolver* resolver) {
    Srt* srt = NULL;
    DType* dtype = resolver->initialized_dtype;

    IntegerLiteral* zero_iliteral = new_signed_iliteral(INTEGER_INT, 0);
    srt = new_iliteral_srt(new_integer_dtype(DTYPE_INT), zero_iliteral);
    if (!dtype_equals(dtype, srt->dtype)) {
        srt = new_dtyped_srt(SRT_CAST_EXPR, dtype_copy(dtype), 1, srt);
    }

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
