#include "./symtab.h"
#include "../common/common.h"

#include <stdlib.h>
#include <string.h>

Symbol* new_symbol(char* name, CType* ctype, int memory_offset);
void delete_symbol(Symbol* symbol);

SymbolTable* new_symboltable() {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->_symbol_map = new_map();
    table->_memory_size = 0;
    table->_outer_scope = NULL;
    return table;
}

void delete_symboltable(SymbolTable* table) {
    if (table->_outer_scope != NULL) delete_symboltable(table->_outer_scope);
    delete_map(table->_symbol_map, delete_str, delete_symbol);
    free(table);
}

int symboltable_can_define(SymbolTable* table, char* name) {
    return map_get(table->_symbol_map, name, str_hash, str_comp) == NULL;
}

Symbol* symboltable_define(SymbolTable* table, char* name, CType* ctype) {
    if (!symboltable_can_define(table, name)) return NULL;
    table->_memory_size += ctype_size(ctype);
    Symbol* symbol = new_symbol(name, ctype, table->_memory_size);
    char* symbol_name = new_string(name);
    map_set(table->_symbol_map, symbol_name, symbol, str_hash, str_comp, delete_str, delete_symbol);
    return symbol;
}

Symbol* symboltable_search(SymbolTable* table, char* name) {
    Symbol* symbol = NULL;
    while (table != NULL) {
        symbol = map_get(table->_symbol_map, name, str_hash, str_comp);
        if (symbol != NULL) break;
        table = table->_outer_scope;
    }
    return symbol;
}

SymbolTable* symboltable_enter_scope(SymbolTable* table) {
    SymbolTable* inner_table = new_symboltable();
    if (table != NULL) {
        inner_table->_memory_size = table->_memory_size;
        inner_table->_outer_scope = table;
    }
    return inner_table;
}

SymbolTable* symboltable_exit_scope(SymbolTable* table) {
    SymbolTable* outer_table = table->_outer_scope;
    if (outer_table != NULL) outer_table->_memory_size = table->_memory_size;
    delete_map(table->_symbol_map, delete_str, delete_symbol);
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
