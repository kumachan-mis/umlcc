#include "./declaration.h"
#include "../common/common.h"
#include "../dtype/dtype.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_decl_list_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    append_children_code(codegen, codes);
    return codes;
}

Vector* gen_init_decl_code(Codegen* codegen) {
    Vector* codes = new_vector(&t_immc);
    append_child_code(codegen, codes, 0);
    return codes;
}

Vector* gen_decl_code(Codegen* codegen) {
    Srt* srt = codegen->srt;

    SymbolTable* table = codegen->global_table;
    if (codegen->local_table != NULL) table = codegen->local_table;

    char* table_ident_name = new_string(srt->ident_name);
    Dtype* table_dtype = dtype_copy(srt->dtype);
    symboltable_define(table, table_ident_name, table_dtype);

    return new_vector(&t_immc);
}
