#include "./data.h"
#include "../immc/immc.h"
#include "./util.h"

Vector* gen_data_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    switch (immc->data->type) {
        case IMMC_DATA_BYTE: {
            IntegerLiteral* iliteral = iliteral_copy(immc->data->iliteral);
            vector_push(codes, new_int_data_x64(X64_DATA_BYTE, iliteral));
            break;
        }
        case IMMC_DATA_WORD: {
            IntegerLiteral* iliteral = iliteral_copy(immc->data->iliteral);
            vector_push(codes, new_int_data_x64(X64_DATA_WORD, iliteral));
            break;
        }
        case IMMC_DATA_LONG: {
            IntegerLiteral* iliteral = iliteral_copy(immc->data->iliteral);
            vector_push(codes, new_int_data_x64(X64_DATA_LONG, iliteral));
            break;
        }
        case IMMC_DATA_QUAD: {
            IntegerLiteral* iliteral = iliteral_copy(immc->data->iliteral);
            vector_push(codes, new_int_data_x64(X64_DATA_QUAD, iliteral));
            break;
        }
        case IMMC_DATA_ZERO: {
            IntegerLiteral* iliteral = iliteral_copy(immc->data->iliteral);
            vector_push(codes, new_int_data_x64(X64_DATA_ZERO, iliteral));
            break;
        }
        case IMMC_DATA_STR: {
            StringLiteral* sliteral = sliteral_copy(immc->data->sliteral);
            vector_push(codes, new_str_data_x64(X64_DATA_STR, sliteral));
            break;
        }
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
