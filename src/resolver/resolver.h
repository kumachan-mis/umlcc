#ifndef UMLCC_RESOLVER_H
#define UMLCC_RESOLVER_H

#include "../ast/ast.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"

typedef struct Resolver {
    Ast* ast;
    SymbolTable* global_table;
    SymbolTable* local_table;
    CType* shared_ctype;
} Resolver;

Resolver* new_resolver(Ast* ast);
void delete_resolver(Resolver* resolver);
Srt* resolver_resolve_semantics(Resolver* resolver);

#endif
