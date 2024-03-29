#ifndef UMLCC_IMMCGEN_H
#define UMLCC_IMMCGEN_H

#include "../immc/immc.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"
#include "../tagtab/tagtab.h"
#include "../vector/vector.h"

typedef struct Immcgen {
    Srt* srt;
    SymbolTable* symbol_table;
    TagTable* tag_table;
    ImmcSuffix expr_reg_suffix;
    ImmcRegister* expr_reg;
    int next_reg_id;
    DType* initialized_dtype;
    int initialized_offset;
    int continue_label_id;
    int break_label_id;
    int return_label_id;
    Vector* case_label_values;
    int default_label_id;
    int label_id;
} Immcgen;

Immcgen* new_immcgen(Srt* srt);
Vector* immcgen_generate_immcode(Immcgen* immcgen);
void delete_immcgen(Immcgen* immcgen);

#endif
