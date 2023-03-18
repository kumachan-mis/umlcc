#ifndef UMLCC_SYMBOL_H
#define UMLCC_SYMBOL_H

#include "../common/type.h"
#include "../dtype/dtype.h"
#include "../literal/iliteral.h"

typedef enum SymbolType {
    SYMBOL_LABEL,
    SYMBOL_MEM,
    SYMBOL_INT,
} SymbolType;

typedef struct Symbol {
    SymbolType type;
    char* name;
    DType* dtype;
    int memory_offset;
    IntegerLiteral* iliteral;
} Symbol;

extern BaseType t_symbol;

Symbol* new_label_symbol(char* name, DType* dtype);
Symbol* new_memory_symbol(char* name, DType* dtype, int memory_offset);
Symbol* new_integer_symbol(char* name, DType* dtype, IntegerLiteral* iliteral);
Symbol* symbol_copy(Symbol* symbol);
void delete_symbol(Symbol* symbol);

#endif
