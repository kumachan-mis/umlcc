#ifndef UMLCC_SYMTAB_H
#define UMLCC_SYMTAB_H

#include "../map/map.h"
#include "./symbol.h"

typedef struct SymbolTable SymbolTable;

struct SymbolTable {
    Map* symbol_map;
    int memory_nbytes;
    SymbolTable* outer_scope;
};

SymbolTable* new_symboltable(void);
SymbolTable* symboltable_copy(SymbolTable* table);
int symboltable_can_define(SymbolTable* table, char* name);
Symbol* symboltable_define_label(SymbolTable* table, char* name, DType* dtype);
Symbol* symboltable_define_memory(SymbolTable* table, char* name, DType* dtype);
Symbol* symboltable_define_integer(SymbolTable* table, char* name, DType* dtype, IntegerLiteral* iliteral);
Symbol* symboltable_search(SymbolTable* table, char* name);
SymbolTable* symboltable_enter_scope(SymbolTable* table);
SymbolTable* symboltable_exit_scope(SymbolTable* table);
void delete_symboltable(SymbolTable* table);

#endif
