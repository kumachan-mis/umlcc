#ifndef UMLCC_SYMTAB_H
#define UMLCC_SYMTAB_H

#include "../map/map.h"
#include "./symbol.h"

typedef struct SymbolTable {
    Map* symbol_map;
    int memory_size;
    struct SymbolTable* outer_scope;
} SymbolTable;

SymbolTable* new_symboltable();
void delete_symboltable(SymbolTable* table);
int symboltable_can_define(SymbolTable* table, char* name);
Symbol* symboltable_define(SymbolTable* table, char* name, CType* ctype);
Symbol* symboltable_search(SymbolTable* table, char* name);
SymbolTable* symboltable_enter_scope(SymbolTable* table);
SymbolTable* symboltable_exit_scope(SymbolTable* table);

#endif
