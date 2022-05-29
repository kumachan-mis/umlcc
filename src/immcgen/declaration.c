#include "./declaration.h"
#include "../common/type.h"
#include "../dtype/dtype.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_decl_list_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_children_immcode(immcgen, codes);
    return codes;
}

Vector* gen_init_decl_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_child_immcode(immcgen, codes, 0);
    return codes;
}

Vector* gen_decl_immcode(Immcgen* immcgen) {
    Srt* srt = immcgen->srt;

    char* symbol_name = new_string(srt->ident_name);
    Dtype* symbol_dtype = dtype_copy(srt->dtype);

    if (immcgen->local_table == NULL) {
        SymbolTable* table = immcgen->global_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else if (symbol_dtype->type == DTYPE_FUNCUCTION) {
        SymbolTable* table = immcgen->local_table;
        symboltable_define_label(table, symbol_name, symbol_dtype);
    } else {
        SymbolTable* table = immcgen->local_table;
        symboltable_define_memory(table, symbol_name, symbol_dtype);
    }

    return new_vector(&t_immc);
}
