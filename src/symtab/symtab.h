#ifndef _UMLCC_SYMTAB_H_
#define _UMLCC_SYMTAB_H_

#include "../map/map.h"
#include "../type/type.h"

typedef struct _SymbolTable {
    Map* _symbol_map;
    int _memory_size;
    struct _SymbolTable* _outer_scope;
} SymbolTable;

typedef struct _Symbol {
    char* name;
    CType* ctype;
    int memory_offset;
} Symbol;

SymbolTable* new_symboltable();
void delete_symboltable(SymbolTable* table);
int symboltable_can_define(SymbolTable* table, char* name);
Symbol* symboltable_define(SymbolTable* table, char* name, CType* ctype);
Symbol* symboltable_search(SymbolTable* table, char* name);
SymbolTable* symboltable_enter_scope(SymbolTable* table);
SymbolTable* symboltable_exit_scope(SymbolTable* table);

#endif
