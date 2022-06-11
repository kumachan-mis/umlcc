#include "./label.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    switch (immc->label->type) {
        case IMMC_LABEL_FUNCTION:
            append_code(codes, "    .text\n");
            break;
        case IMMC_LABEL_VARIABLE:
            append_code(codes, "    .data\n");
            break;
        default:
            break;
    }

    switch (immc->label->visibility) {
        case IMMC_VIS_GLOBAL:
            append_code(codes, "    .globl %s\n", immc->label->name);
            break;
        case IMMC_VIS_LOCAL:
            append_code(codes, "    .local %s\n", immc->label->name);
            break;
        default:
            break;
    }

    append_code(codes, "%s:\n", immc->label->name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
