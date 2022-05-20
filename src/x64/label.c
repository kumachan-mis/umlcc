#include "./label.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_global_label_x64code(X64gen* x64gen);
Vector* gen_local_label_x64code(X64gen* x64gen);
Vector* gen_default_label_x64code(X64gen* x64gen);

Vector* gen_label_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    switch (immc->label->visibility) {
        case LABELVIS_GLOBAL:
            return gen_global_label_x64code(x64gen);
        case LABELVIS_LOCAL:
            return gen_local_label_x64code(x64gen);
        case LABELVIS_DEFAULT:
            return gen_default_label_x64code(x64gen);
        default:
            fprintf(stderr, "Error: unexpected label visibility %d\n", immc->label->visibility);
            exit(1);
    }
}

Vector* gen_global_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    append_code(codes, "    .globl %s\n", immc->label->name);
    append_code(codes, "%s:\n", immc->label->name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_local_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    append_code(codes, "    .local %s\n", immc->label->name);
    append_code(codes, "%s:\n", immc->label->name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}
Vector* gen_default_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    append_code(codes, "%s:\n", immc->label->name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}
