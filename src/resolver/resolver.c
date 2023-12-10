#include "./resolver.h"
#include "./external.h"

#include <stdlib.h>

Resolver* new_resolver(Ast* ast) {
    Resolver* resolver = malloc(sizeof(Resolver));
    resolver->ast = ast;
    resolver->trans_unit_srt = NULL;
    resolver->scope_srt = NULL;
    resolver->symbol_table = new_symboltable();
    resolver->tag_table = new_tagtable();
    resolver->inside_loop = 0;
    resolver->switch_cases = NULL;
    resolver->switch_default_exists = 0;
    resolver->return_dtype = NULL;
    resolver->specifier_dtype = NULL;
    resolver->default_enum_value = 0;
    resolver->initialized_dtype = NULL;
    resolver->initialized_offset = -1;
    resolver->is_nested_initializing = 0;
    resolver->sliteral_id = -1;
    resolver->expr_dtype = NULL;
    return resolver;
}

ResolverReturn* resolver_resolve_semantics(Resolver* resolver) {
    return resolve_transration_unit(resolver);
}

void delete_resolver(Resolver* resolver) {
    delete_ast(resolver->ast);
    if (resolver->trans_unit_srt != NULL) {
        delete_srt(resolver->trans_unit_srt);
    }
    if (resolver->scope_srt != NULL) {
        delete_srt(resolver->scope_srt);
    }
    delete_symboltable(resolver->symbol_table);
    delete_tagtable(resolver->tag_table);
    if (resolver->switch_cases != NULL) {
        delete_vector(resolver->switch_cases);
    }
    if (resolver->return_dtype != NULL) {
        delete_dtype(resolver->return_dtype);
    }
    if (resolver->specifier_dtype != NULL) {
        delete_dtype(resolver->specifier_dtype);
    }
    if (resolver->initialized_dtype != NULL) {
        delete_dtype(resolver->initialized_dtype);
    }
    if (resolver->expr_dtype != NULL) {
        delete_dtype(resolver->expr_dtype);
    }
    free(resolver);
}
