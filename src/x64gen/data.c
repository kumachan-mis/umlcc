#include "./data.h"
#include "../immc/immc.h"
#include "./util.h"

Vector* gen_data_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    X64DataType type = X64_DATA_ZERO;
    switch (immc->data->type) {
        case IMMC_DATA_BYTE:
            type = X64_DATA_BYTE;
            break;
        case IMMC_DATA_WORD:
            type = X64_DATA_WORD;
            break;
        case IMMC_DATA_LONG:
            type = X64_DATA_LONG;
            break;
        case IMMC_DATA_QUAD:
            type = X64_DATA_QUAD;
            break;
        case IMMC_DATA_ZERO:
            type = X64_DATA_ZERO;
            break;
    }

    vector_push(codes, new_data_x64(type, immc->data->value));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
