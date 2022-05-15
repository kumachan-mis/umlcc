#include "./symbol.h"

#include <stdlib.h>

BaseType t_symbol = {
    .copy_object = symbol_copy,
    .delete_object = delete_symbol,
};

Symbol* new_symbol(char* name, CType* ctype, int memory_offset) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->name = name;
    symbol->ctype = ctype;
    symbol->memory_offset = memory_offset;
    return symbol;
}

Symbol* symbol_copy(Symbol* symbol) {
    Symbol* copied_symbol = malloc(sizeof(Symbol));
    copied_symbol->name = new_string(symbol->name);
    copied_symbol->ctype = ctype_copy(symbol->ctype);
    copied_symbol->memory_offset = symbol->memory_offset;
    return copied_symbol;
}

void delete_symbol(Symbol* symbol) {
    free(symbol->name);
    delete_ctype(symbol->ctype);
    free(symbol);
}
