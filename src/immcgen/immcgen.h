#ifndef UMLCC_IMMCGEN_H
#define UMLCC_IMMCGEN_H

#include "../immc/immc.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"
#include "../vector/vector.h"

typedef struct Immcgen {
    Srt* srt;
    SymbolTable* global_table;
    SymbolTable* local_table;
    ImmcSuffix expr_reg_suffix;
    int expr_reg_id;
    int next_reg_id;
    Dtype* initialized_dtype;
    int initialized_offset;
    int return_label_id;
    int label_id;
} Immcgen;

Immcgen* new_immcgen(Srt* srt);
Vector* immcgen_generate_immcode(Immcgen* immcgen);
void delete_immcgen(Immcgen* immcgen);

#endif
