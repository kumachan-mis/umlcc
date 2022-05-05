#include "./statement.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>

Vector* gen_compound_stmt_code(Codegen* codegen) {
    return gen_children_code(codegen);
}

Vector* gen_expression_stmt_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, (void (*)(void* item))delete_immc);

    codegen->_srt = srt;
    return codes;
}
