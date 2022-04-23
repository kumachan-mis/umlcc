#ifndef _UMLCC_GEN_H_
#define _UMLCC_GEN_H_

#include "../vector/vector.h"
#include "../srt/srt.h"
#include "../symtab/symtab.h"


typedef struct _Codegen {
    Srt*         _srt;
    SymbolTable* _table;
} Codegen;

Codegen* new_codegen(Srt* srt);
void     delete_codegen(Codegen* codegen);
Vector*  codegen_generate_code(Codegen* codegen);

#endif
