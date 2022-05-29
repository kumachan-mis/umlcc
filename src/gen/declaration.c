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

    char* symbol_name = new_string(srt->ident_name);
    Dtype* symbol_dtype = dtype_copy(srt->dtype);

    if (codegen->local_table == NULL) {
        SymbolTable* table = codegen->global_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else if (symbol_dtype->type == DTYPE_FUNCUCTION) {
        SymbolTable* table = codegen->local_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else {
        SymbolTable* table = codegen->local_table;
        symboltable_define_memory(table, symbol_name, symbol_dtype);
    }

    return new_vector(&t_immc);
}
