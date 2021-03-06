#include "./symbol.h"

#include <stdlib.h>

BaseType t_symbol = {
    .copy_object = (void* (*)(void*))symbol_copy,
    .delete_object = (void (*)(void*))delete_symbol,
};

Symbol* new_memory_symbol(char* name, Dtype* dtype, int memory_offset) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->type = SYMBOL_MEM;
    symbol->name = name;
    symbol->dtype = dtype;
    symbol->memory_offset = memory_offset;
    return symbol;
}

Symbol* new_label_symbol(char* name, Dtype* dtype) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->type = SYMBOL_LABEL;
    symbol->name = name;
    symbol->dtype = dtype;
    symbol->memory_offset = -1;
    return symbol;
}

Symbol* symbol_copy(Symbol* symbol) {
    Symbol* copied_symbol = malloc(sizeof(Symbol));
    copied_symbol->type = symbol->type;
    copied_symbol->name = new_string(symbol->name);
    copied_symbol->dtype = dtype_copy(symbol->dtype);
    copied_symbol->memory_offset = symbol->memory_offset;
    return copied_symbol;
}

void delete_symbol(Symbol* symbol) {
    free(symbol->name);
    delete_dtype(symbol->dtype);
    free(symbol);
}
