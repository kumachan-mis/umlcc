#include "./resolver.h"
#include "./external.h"

#include <stdlib.h>

Resolver* new_resolver(Ast* ast) {
    Resolver* resolver = malloc(sizeof(Resolver));
    resolver->ast = ast;
    resolver->global_table = new_symboltable();
    resolver->local_table = NULL;
    resolver->specifier_dtype = NULL;
    resolver->initialized_dtype = NULL;
    resolver->initialized_offset = -1;
    resolver->function_dtype = NULL;
    return resolver;
}

Srt* resolver_resolve_semantics(Resolver* resolver) {
    return resolve_transration_unit(resolver);
}

void delete_resolver(Resolver* resolver) {
    delete_ast(resolver->ast);
    delete_symboltable(resolver->global_table);
    if (resolver->local_table != NULL) delete_symboltable(resolver->local_table);
    if (resolver->specifier_dtype != NULL) delete_dtype(resolver->specifier_dtype);
    if (resolver->initialized_dtype != NULL) delete_dtype(resolver->initialized_dtype);
    if (resolver->function_dtype != NULL) delete_dtype(resolver->function_dtype);
    free(resolver);
}
