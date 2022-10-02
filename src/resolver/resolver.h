#ifndef UMLCC_RESOLVER_H
#define UMLCC_RESOLVER_H

#include "../ast/ast.h"
#include "../symtab/symtab.h"
#include "./resolverret.h"

typedef struct Resolver {
    Ast* ast;
    Srt* trans_unit_srt;
    SymbolTable* symbol_table;
    DType* return_dtype;
    DType* specifier_dtype;
    DType* initialized_dtype;
    int initialized_offset;
    int is_nested_initializing;
    int sliteral_id;
    DType* call_dtype;
} Resolver;

Resolver* new_resolver(Ast* ast);
ResolverReturn* resolver_resolve_semantics(Resolver* resolver);
void delete_resolver(Resolver* resolver);

#endif
