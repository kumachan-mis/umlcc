#ifndef _UMLCC_RESOLVER_H_
#define _UMLCC_RESOLVER_H_

#include "../ast/ast.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"


typedef struct _Resolver {
    Ast*         _ast;
    SymbolTable* _table;
} Resolver;

Resolver* new_resolver(Ast* _ast);
void      delete_resolver(Resolver* resolver);
Srt*      resolver_resolve_semantics(Resolver* resolver);

#endif
