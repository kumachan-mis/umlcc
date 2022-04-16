#include "./symtab.h"

#include <stdlib.h>
#include <string.h>


Symbol* new_symbol(char* name, int memory_offset);
void    delete_symbol(Symbol* symbol);

SymbolTable* new_symboltable() {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->_symbol_map = new_map();
    table->_memory_offset = 0;
    table->_outer_scope = NULL;
}

void delete_symboltable(SymbolTable* table) {
    if (table->_outer_scope != NULL) {
        delete_symboltable(table->_outer_scope);
    }
    delete_map(table->_symbol_map, delete_symbol);
    free(table);
}

void symboltable_define_symbol(SymbolTable* table, char* name) {
    if (map_get(table->_symbol_map, name) != NULL) return;
    char* key_name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(key_name, name);
    Symbol* symbol = new_symbol(name, table->_memory_offset);
    map_set(table->_symbol_map, key_name, symbol, delete_symbol);
    table->_memory_offset += 4;
}

Symbol* symboltable_search_symbol(SymbolTable* table, char* name) {
    Symbol* symbol = NULL;
    while (table != NULL) {
        symbol = map_get(table->_symbol_map, name);
        if (symbol != NULL) break;
        table = table->_outer_scope;
    }
    return symbol;
}

SymbolTable* symboltable_enter_scope(SymbolTable* table) {
    SymbolTable* inner_table = new_symboltable();
    inner_table->_memory_offset = table->_memory_offset;
    inner_table->_outer_scope = table;
    return inner_table;
}

SymbolTable* symboltable_exit_scope(SymbolTable* table) {
    SymbolTable* outer_table = table->_outer_scope;
    outer_table->_memory_offset = table->_memory_offset;
    delete_map(table->_symbol_map, delete_symbol);
    free(table);
    return outer_table;
}

Symbol* new_symbol(char* name, int memory_offset) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->name = name;
    symbol->memory_offset = memory_offset;
    return symbol;
}

void delete_symbol(Symbol* symbol) {
    free(symbol->name);
    free(symbol);
}
