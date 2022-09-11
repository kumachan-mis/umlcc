#ifndef UMLCC_RESOLVER_H
#define UMLCC_RESOLVER_H

#include "../ast/ast.h"
#include "../symtab/symtab.h"
#include "./resolverret.h"

typedef struct Resolver {
    Ast* ast;
    Srt* trans_unit_srt;
    SymbolTable* global_table;
    SymbolTable* local_table;
    Dtype* return_dtype;
    Dtype* specifier_dtype;
    Dtype* initialized_dtype;
    int initialized_offset;
    int sliteral_id;
    Dtype* call_dtype;
} Resolver;

Resolver* new_resolver(Ast* ast);
ResolverReturn* resolver_resolve_semantics(Resolver* resolver);
void delete_resolver(Resolver* resolver);

#endif
