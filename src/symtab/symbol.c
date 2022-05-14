#include "./symbol.h"

#include <stdlib.h>

BaseType t_symbol = {
    .delete_object = delete_symbol,
};

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
