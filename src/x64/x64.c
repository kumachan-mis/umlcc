#include "./x64.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./consts.h"
#include "./inst.h"
#include "./label.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_function_x64code(X64gen* x64gen);

X64gen* new_x64gen(Vector* immcs, Vector* liveseqs) {
    X64gen* x64gen = malloc(sizeof(X64gen));
    x64gen->_immcs = immcs;
    x64gen->_immcs = liveseqs;
    x64gen->_index = 0;
    x64gen->_evacuation_count = 0;
    return x64gen;
}

void delete_x64gen(X64gen* x64gen) {
    delete_vector(x64gen->_immcs);
    free(x64gen);
}

Vector* x64gen_generate_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Vector* sub_codes = NULL;

    int immcs_len = vector_size(x64gen->_immcs);
    while (1) {
        if (x64gen->_index >= immcs_len) break;

        Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
        switch (immc->label->type) {
            case LABEL_FUNCTION:
                sub_codes = gen_function_x64code(x64gen);
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

    x64gen->_evacuation_count = 0;

    sub_codes = gen_label_x64code(x64gen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes);

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(head_codes, sub_codes);
    delete_vector(sub_codes);

    Vector* body_codes = new_vector(&t_string);
    while (1) {
        Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
        if (immc->type == IMMC_INST && immc->inst->type == INST_LEAVE) break;

        switch (immc->type) {
            case IMMC_INST:
                sub_codes = gen_inst_x64code(x64gen);
                break;
            case IMMC_LABEL:
                sub_codes = gen_label_x64code(x64gen);
                break;
        }
        vector_extend(body_codes, sub_codes);
        delete_vector(sub_codes);
    }

    int evacuation_count = x64gen->_evacuation_count;
    for (int i = 0; i < evacuation_count; i++) {
        append_code(head_codes, "\tpushq\t%s\n", QREG_NAMES[CALLEE_SAVED_REG_IDS[i]]);
    }
    if (evacuation_count % 2 == 1) {
        append_code(head_codes, "\tsubq\t$%d, %s\n", 8, QREG_NAMES[SP_REG_ID]);
        append_code(tail_codes, "\taddq\t$%d, %s\n", 8, QREG_NAMES[SP_REG_ID]);
    }
    for (int i = evacuation_count - 1; i >= 0; i--) {
        append_code(tail_codes, "\tpopq\t%s\n", QREG_NAMES[CALLEE_SAVED_REG_IDS[i]]);
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

    x64gen->_evacuation_count = 0;
    return codes;
}
