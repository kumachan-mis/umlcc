#ifndef UMLCC_SYMBOL_H
#define UMLCC_SYMBOL_H

#include "../common/common.h"
#include "../dtype/dtype.h"

typedef struct Symbol {
    char* name;
    Dtype* dtype;
    int memory_offset;
} Symbol;

extern BaseType t_symbol;

Symbol* new_symbol(char* name, Dtype* dtype, int memory_offset);
Symbol* symbol_copy(Symbol* symbol);
void delete_symbol(Symbol* symbol);

#endif
