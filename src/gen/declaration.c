#include "./declaration.h"
#include "./util.h"
#include "../type/type.h"
#include "../common/common.h"

#include <stdio.h>


Vector* gen_decl_code(Codegen* codegen) {
    Srt* srt = codegen->_srt;
    char* table_ident_name = string_copy(srt->ident_name);
    CType* table_ctype = ctype_copy(srt->ctype);
    symboltable_define(codegen->_local_table, table_ident_name, table_ctype);
    return new_vector();
}
