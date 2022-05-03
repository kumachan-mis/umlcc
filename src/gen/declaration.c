#include "./declaration.h"
#include "../common/common.h"
#include "../imml/imml.h"
#include "../type/type.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_decl_list_code(Codegen* codegen) {
    return gen_children_code(codegen);
}

Vector* gen_init_decl_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immlcode);

    codegen->_srt = srt;
    return codes;
}

Vector* gen_decl_code(Codegen* codegen) {
    Srt* srt = codegen->_srt;

    SymbolTable* table = codegen->_global_table;
    if (codegen->_local_table != NULL) table = codegen->_local_table;

    char* table_ident_name = string_copy(srt->ident_name);
    CType* table_ctype = ctype_copy(srt->ctype);
    symboltable_define(table, table_ident_name, table_ctype);

    return new_vector();
}
