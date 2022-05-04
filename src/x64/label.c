#include "./label.h"
#include "../immc/immc.h"
#include "./util.h"

Vector* gen_global_label_x64code(X64gen* x64gen);
Vector* gen_local_label_x64code(X64gen* x64gen);
Vector* gen_default_label_x64code(X64gen* x64gen);

Vector* gen_label_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    switch (immc->label->visibility) {
        case VISIBILITY_GLOBAL:
            return gen_global_label_x64code(x64gen);
        case VISIBILITY_LOCAL:
            return gen_local_label_x64code(x64gen);
        case VISIBILITY_DEFAULT:
            return gen_default_label_x64code(x64gen);
    }
}

Vector* gen_global_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    append_code(codes, "    .globl %s\n", immc->label->label_name);
    append_code(codes, "%s:\n", immc->label->label_name);
    return codes;
}

Vector* gen_local_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    append_code(codes, "    .local %s\n", immc->label->label_name);
    append_code(codes, "%s:\n", immc->label->label_name);
    return codes;
}
Vector* gen_default_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    append_code(codes, "%s:\n", immc->label->label_name);
    return codes;
}
