#include "./declaration.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "../type/type.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_decl_list_code(Codegen* codegen) {
    Vector* codes = new_vector();
    append_children_code(codegen, codes);
    return codes;
}

Vector* gen_init_decl_code(Codegen* codegen) {
    Vector* codes = new_vector();
    append_child_code(codegen, codes, 0);
    return codes;
}

Vector* gen_decl_code(Codegen* codegen) {
    Srt* srt = codegen->_srt;

    SymbolTable* table = codegen->_global_table;
    if (codegen->_local_table != NULL) table = codegen->_local_table;

    char* table_ident_name = new_string(srt->ident_name);
    CType* table_ctype = ctype_copy(srt->ctype);
    symboltable_define(table, table_ident_name, table_ctype);

    return new_vector();
}
