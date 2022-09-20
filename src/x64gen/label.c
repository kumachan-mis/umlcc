#include "./label.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_label_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    X64LabelType type = X64_LABEL_NORMAL;
    switch (immc->label->type) {
        case IMMC_LABEL_FUNCTION:
            type = X64_LABEL_FUNCTION;
            break;
        case IMMC_LABEL_VARIABLE:
            type = X64_LABEL_VARIABLE;
            break;
        case IMMC_LABEL_NORMAL:
            type = X64_LABEL_NORMAL;
            break;
    }

    X64Visibility visibility = X64_VIS_NONE;
    switch (immc->label->visibility) {
        case IMMC_VIS_GLOBAL:
            visibility = X64_VIS_GLOBAL;
            break;
        case IMMC_VIS_LOCAL:
            visibility = X64_VIS_LOCAL;
            break;
        case IMMC_VIS_NONE:
            visibility = X64_VIS_NONE;
            break;
    }

    vector_push(codes, new_label_x64(type, visibility, new_string(immc->label->name)));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
