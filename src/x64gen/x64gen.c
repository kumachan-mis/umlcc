#include "./x64gen.h"
#include "../common/type.h"
#include "../immc/immc.h"
#include "./data.h"
#include "./inst.h"
#include "./label.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_function_x64code(X64gen* x64gen);
Vector* gen_variable_x64code(X64gen* x64gen);

X64gen* new_x64gen(Vector* immcs, Vector* liveseqs) {
    X64gen* x64gen = malloc(sizeof(X64gen));
    x64gen->immcs = immcs;
    x64gen->liveseqs = liveseqs;
    x64gen->index = 0;
    x64gen->evacuation_count = 0;
    return x64gen;
}

Vector* x64gen_generate_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Vector* sub_codes = NULL;

    int immcs_len = vector_size(x64gen->immcs);
    while (1) {
        if (x64gen->index >= immcs_len) break;

        Immc* immc = vector_at(x64gen->immcs, x64gen->index);
        switch (immc->label->type) {
            case IMMC_LABEL_FUNCTION:
                sub_codes = gen_function_x64code(x64gen);
                break;
            case IMMC_LABEL_VARIABLE:
                sub_codes = gen_variable_x64code(x64gen);
                break;
            default:
                fprintf(stderr, "Error: unexpected external label type %d\n", immc->label->type);
                exit(1);
        }
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes);
    }

    return codes;
}

Vector* gen_function_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);

    Vector* head_codes = new_vector(&t_string);
    Vector* tail_codes = new_vector(&t_string);
    Vector* sub_codes = NULL;

    x64gen->evacuation_count = 0;

    sub_codes = gen_label_x64code(x64gen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes);

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(head_codes, sub_codes);
    delete_vector(sub_codes);

    Vector* body_codes = new_vector(&t_string);
    while (1) {
        Immc* immc = vector_at(x64gen->immcs, x64gen->index);
        if (immc->type == IMMC_INST && immc->inst->type == IMMC_INST_LEAVE) break;

        switch (immc->type) {
            case IMMC_INST:
                sub_codes = gen_inst_x64code(x64gen);
                break;
            case IMMC_DATA:
                sub_codes = gen_data_x64code(x64gen);
                break;
            case IMMC_LABEL:
                sub_codes = gen_label_x64code(x64gen);
                break;
            default:
                fprintf(stderr, "Error: unexpected immc type %d\n", immc->type);
                exit(1);
        }
        vector_extend(body_codes, sub_codes);
        delete_vector(sub_codes);
    }

    int evacuation_count = x64gen->evacuation_count;
    for (int i = 0; i < evacuation_count; i++) {
        X64Ope* src = new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[i]);
        vector_push(head_codes, new_inst_x64(X64_INST_PUSHX, src, NULL));
    }
    if (evacuation_count % 2 == 1) {
        X64Ope* src = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8);
        X64Ope* dst = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(head_codes, new_inst_x64(X64_INST_SUBX, src, dst));
    }

    if (evacuation_count % 2 == 1) {
        X64Ope* src = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8);
        X64Ope* dst = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(head_codes, new_inst_x64(X64_INST_ADDX, src, dst));
    }
    for (int i = evacuation_count - 1; i >= 0; i--) {
        X64Ope* src = new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[i]);
        vector_push(head_codes, new_inst_x64(X64_INST_POPX, src, NULL));
    }

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(tail_codes, sub_codes);
    delete_vector(sub_codes);

    vector_extend(codes, head_codes);
    delete_vector(head_codes);
    vector_extend(codes, body_codes);
    delete_vector(body_codes);
    vector_extend(codes, tail_codes);
    delete_vector(tail_codes);

    x64gen->evacuation_count = 0;
    return codes;
}

Vector* gen_variable_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Vector* sub_codes = NULL;

    sub_codes = gen_label_x64code(x64gen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes);

    while (1) {
        Immc* immc = vector_at(x64gen->immcs, x64gen->index);
        if (immc == NULL || (immc->type == IMMC_LABEL && immc->label->type != IMMC_LABEL_NORMAL)) {
            break;
        }

        switch (immc->type) {
            case IMMC_DATA:
                sub_codes = gen_data_x64code(x64gen);
                break;
            case IMMC_LABEL:
                sub_codes = gen_label_x64code(x64gen);
                break;
            default:
                fprintf(stderr, "Error: unexpected immc type %d\n", immc->type);
                exit(1);
        }
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes);
    }

    return codes;
}

void delete_x64gen(X64gen* x64gen) {
    delete_vector(x64gen->immcs);
    delete_vector(x64gen->liveseqs);
    free(x64gen);
}
