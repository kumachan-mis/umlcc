#include "./symbol.h"

#include <stdlib.h>

BaseType t_symbol = {
    .copy_object = (void* (*)(void*))symbol_copy,
    .delete_object = (void (*)(void*))delete_symbol,
};

Symbol* new_base_symbol(SymbolType type, char* name, DType* dtype);

Symbol* new_label_symbol(char* name, DType* dtype) {
    Symbol* symbol = new_base_symbol(SYMBOL_LABEL, name, dtype);
    return symbol;
}

Symbol* new_memory_symbol(char* name, DType* dtype, int memory_offset) {
    Symbol* symbol = new_base_symbol(SYMBOL_MEM, name, dtype);
    symbol->memory_offset = memory_offset;
    return symbol;
}

Symbol* new_integer_symbol(char* name, DType* dtype, IntegerLiteral* iliteral) {
    Symbol* symbol = new_base_symbol(SYMBOL_INT, name, dtype);
    symbol->iliteral = iliteral;
    return symbol;
}

Symbol* new_base_symbol(SymbolType type, char* name, DType* dtype) {
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->type = type;
    symbol->dtype = dtype;
    symbol->name = name;
    symbol->memory_offset = -1;
    symbol->iliteral = NULL;
    return symbol;
}

Symbol* symbol_copy(Symbol* symbol) {
    Symbol* copied_symbol = malloc(sizeof(Symbol));
    copied_symbol->type = symbol->type;
    copied_symbol->name = new_string(symbol->name);
    copied_symbol->dtype = dtype_copy(symbol->dtype);
    copied_symbol->memory_offset = symbol->memory_offset;
    copied_symbol->iliteral = NULL;
    if (symbol->iliteral != NULL) {
        copied_symbol->iliteral = iliteral_copy(symbol->iliteral);
    }
    return copied_symbol;
}

void delete_symbol(Symbol* symbol) {
    free(symbol->name);
    delete_dtype(symbol->dtype);
    if (symbol->iliteral != NULL) {
        delete_iliteral(symbol->iliteral);
    }
    free(symbol);
}
