#include "./symtab.h"
#include "../common/type.h"

#include <stdlib.h>
#include <string.h>

SymbolTable* new_symboltable() {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->symbol_map = new_map(&t_hashable_string, &t_symbol);
    table->memory_size = 0;
    table->outer_scope = NULL;
    return table;
}

SymbolTable* symboltable_copy(SymbolTable* table) {
    SymbolTable* copied_table = malloc(sizeof(SymbolTable));
    copied_table->symbol_map = map_copy(table->symbol_map);
    copied_table->memory_size = table->memory_size;
    copied_table->outer_scope = NULL;
    if (table->outer_scope != NULL) {
        copied_table->outer_scope = symboltable_copy(table->outer_scope);
    }
    return copied_table;
}

int symboltable_can_define(SymbolTable* table, char* name) {
    return map_get(table->symbol_map, name) == NULL;
}

Symbol* symboltable_define_memory(SymbolTable* table, char* name, Dtype* dtype) {
    if (!symboltable_can_define(table, name)) return NULL;
    table->memory_size += dtype_size(dtype);
    Symbol* symbol = new_memory_symbol(name, dtype, table->memory_size);
    char* symbol_name = new_string(name);
    map_add(table->symbol_map, symbol_name, symbol);
    return symbol;
}

Symbol* symboltable_define_label(SymbolTable* table, char* name, Dtype* dtype) {
    if (!symboltable_can_define(table, name)) return NULL;
    Symbol* symbol = new_label_symbol(name, dtype);
    char* symbol_name = new_string(name);
    map_add(table->symbol_map, symbol_name, symbol);
    return symbol;
}

Symbol* symboltable_search(SymbolTable* table, char* name) {
    Symbol* symbol = NULL;
    while (table != NULL) {
        symbol = map_get(table->symbol_map, name);
        if (symbol != NULL) break;
        table = table->outer_scope;
    }
    return symbol;
}

SymbolTable* symboltable_enter_scope(SymbolTable* table) {
    SymbolTable* inner_table = new_symboltable();
    if (table != NULL) {
        inner_table->memory_size = table->memory_size;
        inner_table->outer_scope = table;
    }
    return inner_table;
}

SymbolTable* symboltable_exit_scope(SymbolTable* table) {
    SymbolTable* outer_table = table->outer_scope;
    if (outer_table != NULL) outer_table->memory_size = table->memory_size;
    delete_map(table->symbol_map);
    free(table);
    return outer_table;
}

void delete_symboltable(SymbolTable* table) {
    if (table->outer_scope != NULL) delete_symboltable(table->outer_scope);
    delete_map(table->symbol_map);
    free(table);
}
