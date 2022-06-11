#include "./data.h"
#include "./util.h"

Vector* gen_data_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    switch (immc->data->type) {
        case IMMC_DATA_BYTE:
            append_code(codes, "\t.byte %d\n", immc->data->value);
            break;
        case IMMC_DATA_WORD:
            append_code(codes, "\t.word %d\n", immc->data->value);
            break;
        case IMMC_DATA_LONG:
            append_code(codes, "\t.long %d\n", immc->data->value);
            break;
        case IMMC_DATA_QUAD:
            append_code(codes, "\t.quad %d\n", immc->data->value);
            break;
        case IMMC_DATA_ZERO:
            append_code(codes, "\t.zero %d\n", immc->data->value);
            break;
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
