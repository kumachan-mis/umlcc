#ifndef _UMLCC_SYMTAB_H_
#define _UMLCC_SYMTAB_H_

#include "../map/map.h"


typedef struct _SymbolTable {
    Map*                 _symbol_map;
    int                  _memory_offset;
    struct _SymbolTable* _outer_scope;
} SymbolTable;

typedef struct _Symbol {
    char* name;
    int   memory_offset;
} Symbol;


SymbolTable* new_symboltable();
void         delete_symboltable(SymbolTable* table);
void         symboltable_define_symbol(SymbolTable* table, char* name);
Symbol*      symboltable_search_symbol(SymbolTable* table, char* name);
SymbolTable* symboltable_enter_scope(SymbolTable* table);
SymbolTable* symboltable_exit_scope(SymbolTable* table);

#endif
