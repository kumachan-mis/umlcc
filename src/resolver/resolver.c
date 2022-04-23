#include "./resolver.h"
#include "./external.h"

#include <stdlib.h>


Resolver* new_resolver(Ast* _ast) {
    Resolver* resolver = malloc(sizeof(Resolver));
    resolver->_ast = _ast;
    resolver->_table = new_symboltable();
    resolver->_decl_ctype = NULL;
    return resolver;
}

void delete_resolver(Resolver* resolver) {
    delete_ast(resolver->_ast);
    delete_symboltable(resolver->_table);
    if (resolver->_decl_ctype != NULL) {
        delete_ctype(resolver->_decl_ctype);
    }
    free(resolver);
}

Srt* resolver_resolve_semantics(Resolver* resolver) {
    return resolve_transration_unit(resolver);
}
