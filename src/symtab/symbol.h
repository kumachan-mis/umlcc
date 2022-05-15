#ifndef _UMLCC_SYMBOL_H_
#define _UMLCC_SYMBOL_H_

#include "../common/common.h"
#include "../type/type.h"

typedef struct _Symbol {
    char* name;
    CType* ctype;
    int memory_offset;
} Symbol;

extern BaseType t_symbol;

Symbol* new_symbol(char* name, CType* ctype, int memory_offset);
Symbol* symbol_copy(Symbol* symbol);
void delete_symbol(Symbol* symbol);

#endif
