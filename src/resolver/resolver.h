#ifndef UMLCC_RESOLVER_H
#define UMLCC_RESOLVER_H

#include "../ast/ast.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"

typedef struct Resolver {
    Ast* ast;
    Srt* trans_unit_srt;
    SymbolTable* global_table;
    SymbolTable* local_table;
    Dtype* specifier_dtype;
    Dtype* initialized_dtype;
    int initialized_offset;
    int string_literal_id;
    Dtype* function_dtype;
} Resolver;

Resolver* new_resolver(Ast* ast);
Srt* resolver_resolve_semantics(Resolver* resolver);
void delete_resolver(Resolver* resolver);

#endif
