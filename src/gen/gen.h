#ifndef _UMLCC_GEN_H_
#define _UMLCC_GEN_H_

#include "../immc/immc.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"
#include "../vector/vector.h"

typedef struct _Codegen {
    Srt* _srt;
    SymbolTable* _global_table;
    SymbolTable* _local_table;
    ImmcOpeSuffix _virtual_reg_suffix;
    int _virtual_reg_id;
    int _return_label_id;
    int _label_id;
} Codegen;

Codegen* new_codegen(Srt* srt);
void delete_codegen(Codegen* codegen);
Vector* codegen_generate_code(Codegen* codegen);

#endif
