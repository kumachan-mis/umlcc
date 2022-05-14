#include "./util.h"
#include "../immc/immc.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_children_code(Codegen* codegen, Vector* codes) {
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    int num_children = vector_size(srt->children);
    for (int i = 0; i < num_children; i++) {
        codegen->_srt = vector_at(srt->children, i);
        sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, delete_immc);
    }

    codegen->_srt = srt;
}

void append_child_code(Codegen* codegen, Vector* codes, int index) {
    Vector* sub_codes = NULL;
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, index);
    sub_codes = codegen_generate_code(codegen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, delete_immc);

    codegen->_srt = srt;
}

char* create_label_name(int label_id) {
    char* label_name = malloc(20 * sizeof(char));
    sprintf(label_name, "L%d", label_id);
    label_name = realloc(label_name, (strlen(label_name) + 1) * sizeof(char));
    return label_name;
}
