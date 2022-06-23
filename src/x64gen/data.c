#include "./data.h"
#include "../immc/immc.h"
#include "./util.h"

Vector* gen_data_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    switch (immc->data->type) {
        case IMMC_DATA_BYTE:
            vector_push(codes, new_imm_data_x64(X64_DATA_BYTE, immc->data->imm_value));
            break;
        case IMMC_DATA_WORD:
            vector_push(codes, new_imm_data_x64(X64_DATA_WORD, immc->data->imm_value));
            break;
        case IMMC_DATA_LONG:
            vector_push(codes, new_imm_data_x64(X64_DATA_LONG, immc->data->imm_value));
            break;
        case IMMC_DATA_QUAD:
            vector_push(codes, new_imm_data_x64(X64_DATA_QUAD, immc->data->imm_value));
            break;
        case IMMC_DATA_ZERO:
            vector_push(codes, new_imm_data_x64(X64_DATA_ZERO, immc->data->imm_value));
            break;
        case IMMC_DATA_STR: {
            StringLiteral* sliteral = sliteral_copy(immc->data->sliteral);
            vector_push(codes, new_str_data_x64(X64_DATA_STR, sliteral));
            break;
        }
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
