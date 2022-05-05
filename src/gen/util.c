#include "./util.h"
#include "../immc/immc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector* gen_children_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    int num_children = vector_size(srt->children);
    for (int i = 0; i < num_children; i++) {
        codegen->_srt = vector_at(srt->children, i);
        sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, (void (*)(void* item))delete_immc);
    }

    codegen->_srt = srt;
    return codes;
}
