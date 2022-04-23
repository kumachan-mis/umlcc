#include "./declaration.h"
#include "./util.h"
#include "../type/type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Vector* gen_decl_code(Codegen* codegen) {
    Srt* srt = codegen->_srt;
    SymbolTable* table = codegen->_table;
    char* ident_name = malloc((strlen(srt->ident_name) + 1) * sizeof(char));
    strcpy(ident_name, srt->ident_name);
    symboltable_define_symbol(table, ident_name, ctype_copy(srt->ctype));
    return new_vector();
}
