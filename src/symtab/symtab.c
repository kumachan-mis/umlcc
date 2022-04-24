#include "./symtab.h"

#include <stdlib.h>
#include <string.h>


Symbol* new_symbol(char* name, CType* ctype, int memory_offset);
void    delete_symbol(Symbol* symbol);

SymbolTable* new_symboltable() {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->_symbol_map = new_map();
    table->_memory_offset = 0;
    table->_outer_scope = NULL;
    return table;
}

void delete_symboltable(SymbolTable* table) {
    if (table->_outer_scope != NULL) {
        delete_symboltable(table->_outer_scope);
    }
    delete_map(table->_symbol_map, (void (*)(void* value))delete_symbol);
    free(table);
}

void symboltable_define(SymbolTable* table, char* name, CType* ctype) {
    if (map_get(table->_symbol_map, name) != NULL) return;
    if (!symboltable_is_global_scope(table)) {
        table->_memory_offset += ctype_size(ctype);
    }
    Symbol* symbol = new_symbol(name, ctype, table->_memory_offset);
    map_set(table->_symbol_map, name, symbol, (void (*)(void* value))delete_symbol);
}

Symbol* symboltable_search(SymbolTable* table, char* name) {
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
    if (table != NULL) {
        inner_table->_memory_offset = table->_memory_offset;
        inner_table->_outer_scope = table;
    }
    return inner_table;
}

int symboltable_is_global_scope(SymbolTable* table) {
    return table->_outer_scope == NULL;
}

SymbolTable* symboltable_exit_scope(SymbolTable* table) {
    SymbolTable* outer_table = table->_outer_scope;
    if (outer_table != NULL) outer_table->_memory_offset = table->_memory_offset;
    delete_map(table->_symbol_map, (void (*)(void* value))delete_symbol);
    free(table);
    return outer_table;
}

Symbol* new_symbol(char* name, CType* ctype, int memory_offset) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->name = name;
    symbol->ctype = ctype;
    symbol->memory_offset = memory_offset;
    return symbol;
}

void delete_symbol(Symbol* symbol) {
    free(symbol->name);
    delete_ctype(symbol->ctype);
    free(symbol);
}
