#ifndef UMLCC_GEN_H
#define UMLCC_GEN_H

#include "../immc/immc.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"
#include "../vector/vector.h"

typedef struct Codegen {
    Srt* srt;
    SymbolTable* global_table;
    SymbolTable* local_table;
    ImmcOpeSuffix virtual_reg_suffix;
    int return_label_id;
    int virtual_reg_id;
    int label_id;
} Codegen;

Codegen* new_codegen(Srt* srt);
Vector* codegen_generate_code(Codegen* codegen);
void delete_codegen(Codegen* codegen);

#endif
