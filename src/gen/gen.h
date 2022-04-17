#ifndef _UMLCC_GEN_H_
#define _UMLCC_GEN_H_

#include "../vector/vector.h"
#include "../ast/ast.h"
#include "../symtab/symtab.h"


typedef struct _Codegen {
    Ast*         _ast;
    SymbolTable* _table;
    int          _assignee_mode;
} Codegen;

Codegen* new_codegen(Ast* ast);
void     delete_codegen(Codegen* codegen);
Vector*  codegen_generate_code(Codegen* codegen);

#endif
