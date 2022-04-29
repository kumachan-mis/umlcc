#include "./resolver.h"
#include "./external.h"

#include <stdlib.h>

Resolver* new_resolver(Ast* ast) {
    Resolver* resolver = malloc(sizeof(Resolver));
    resolver->_ast = ast;
    resolver->_global_table = new_symboltable();
    resolver->_local_table = NULL;
    resolver->_shared_ctype = NULL;
    return resolver;
}

void delete_resolver(Resolver* resolver) {
    delete_ast(resolver->_ast);
    delete_symboltable(resolver->_global_table);
    if (resolver->_local_table != NULL) { delete_symboltable(resolver->_local_table); }
    if (resolver->_shared_ctype != NULL) { delete_ctype(resolver->_shared_ctype); }
    free(resolver);
}

Srt* resolver_resolve_semantics(Resolver* resolver) {
    return resolve_transration_unit(resolver);
}
