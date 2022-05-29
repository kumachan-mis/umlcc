#ifndef UMLCC_SYMBOL_H
#define UMLCC_SYMBOL_H

#include "../common/type.h"
#include "../dtype/dtype.h"

typedef enum SymbolType {
    SYMBOL_MEM,
    SYMBOL_LABEL,
} SymbolType;

typedef struct Symbol {
    SymbolType type;
    char* name;
    Dtype* dtype;
    int memory_offset;
} Symbol;

extern BaseType t_symbol;

Symbol* new_memory_symbol(char* name, Dtype* dtype, int memory_offset);
Symbol* new_label_symbol(char* name, Dtype* dtype);
Symbol* symbol_copy(Symbol* symbol);
void delete_symbol(Symbol* symbol);

#endif
