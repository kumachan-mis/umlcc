#ifndef UMLCC_RESOLVER_H
#define UMLCC_RESOLVER_H

#include "../ast/ast.h"
#include "../set/set.h"
#include "../symtab/symtab.h"
#include "../tagtab/tagtab.h"
#include "./resolverret.h"

typedef struct Resolver {
    Ast* ast;
    Srt* trans_unit_srt;
    Srt* scope_srt;
    SymbolTable* symbol_table;
    TagTable* tag_table;
    int inside_loop;
    Vector* switch_cases;
    int switch_default_exists;
    DType* return_dtype;
    DType* specifier_dtype;
    int default_enum_value;
    DType* initialized_dtype;
    int initialized_offset;
    int is_nested_initializing;
    int sliteral_id;
    DType* expr_dtype;
} Resolver;

Resolver* new_resolver(Ast* ast);
ResolverReturn* resolver_resolve_semantics(Resolver* resolver);
void delete_resolver(Resolver* resolver);

#endif
