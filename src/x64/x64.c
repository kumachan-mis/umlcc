#include "./x64.h"
#include "../immc/immc.h"
#include "./inst.h"
#include "./label.h"

#include <stdlib.h>

Vector* gen_function_x64code(X64gen* x64gen);

X64gen* new_x64gen(Vector* immcs) {
    X64gen* x64gen = malloc(sizeof(X64gen));
    x64gen->_immcs = immcs;
    x64gen->index = 0;
    return x64gen;
}

void delete_x64gen(X64gen* x64gen) {
    delete_vector(x64gen->_immcs, (void (*)(void* item))delete_immc);
    free(x64gen);
}

Vector* x64gen_generate_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;

    int immcs_len = vector_size(x64gen->_immcs);
    while (1) {
        if (x64gen->index >= immcs_len) break;

        Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
        switch (immc->label->type) {
            case LABEL_FUNCTION:
                sub_codes = gen_function_x64code(x64gen);
                break;
            default:
                break;
        }
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
    }
}

Vector* gen_function_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();

    Vector* head_codes = new_vector();
    Vector* body_codes = new_vector();
    Vector* tail_codes = new_vector();
    Vector* sub_codes = NULL;

    sub_codes = gen_label_x64code(x64gen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(head_codes, sub_codes);
    delete_vector(sub_codes, free);

    int terminated = 0;
    while (!terminated) {
        Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
        switch (immc->type) {
            case IMMC_INST:
                if (immc->inst->type == INST_LEAVE) {
                    terminated = 1;
                    break;
                }
                sub_codes = gen_inst_x64code(x64gen);
                break;
            case IMMC_LABEL:
                sub_codes = gen_label_x64code(x64gen);
                break;
        }
        vector_extend(body_codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    // push calee-saved register
    // pup calee-saved register

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(tail_codes, sub_codes);
    delete_vector(sub_codes, free);

    vector_extend(codes, head_codes);
    delete_vector(head_codes, free);
    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);
    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);

    return codes;
}
