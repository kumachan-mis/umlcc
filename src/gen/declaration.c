#include "./declaration.h"
#include "./util.h"
#include "../type/type.h"
#include "../common/common.h"

#include <stdio.h>


Vector* gen_decl_code(Codegen* codegen) {
    Srt* srt = codegen->_srt;
    SymbolTable* table = codegen->_local_table;
    symboltable_define(table, string_copy(srt->ident_name), ctype_copy(srt->ctype));
    return new_vector();
}
