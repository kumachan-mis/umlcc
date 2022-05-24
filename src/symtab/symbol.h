#ifndef UMLCC_SYMBOL_H
#define UMLCC_SYMBOL_H

#include "../common/common.h"
#include "../type/type.h"

typedef struct Symbol {
    char* name;
    CType* ctype;
    int memory_offset;
} Symbol;

extern BaseType t_symbol;

Symbol* new_symbol(char* name, CType* ctype, int memory_offset);
Symbol* symbol_copy(Symbol* symbol);
void delete_symbol(Symbol* symbol);

#endif
