#include "./x64.h"
#include "../immc/immc.h"
#include "./inst.h"
#include "./label.h"
#include "./consts.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_function_x64code(X64gen* x64gen);
Vector* gen_function_body_x64code(X64gen* x64gen);

X64gen* new_x64gen(Vector* immcs) {
    X64gen* x64gen = malloc(sizeof(X64gen));
    x64gen->_immcs = immcs;
    x64gen->index = 0;
    x64gen->callee_saved_count = 0;
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
                fprintf(stderr, "Error: unexpected external label type %d\n", immc->label->type);
                exit(1);
        }
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    return codes;
}

Vector* gen_function_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();

    Vector* head_codes = new_vector();
    Vector* tail_codes = new_vector();
    Vector* sub_codes = NULL;

    x64gen->callee_saved_count = 0;

    sub_codes = gen_label_x64code(x64gen);
    vector_extend(codes, sub_codes);
    delete_vector(sub_codes, free);

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(head_codes, sub_codes);
    delete_vector(sub_codes, free);

    Vector* body_codes = gen_function_body_x64code(x64gen);

    int callee_saved_count = x64gen->callee_saved_count;
    for (int i = 0; i < callee_saved_count; i++) {
        append_code(head_codes, "\tpushq\t%s\n", callee_reg_name(QREG_NAMES, i));
    }
    if (callee_saved_count % 2 == 1) {
        append_code(head_codes, "\tsubq\t$%d, %s", 8, stackptr_reg(QREG_NAMES));
        append_code(tail_codes, "\taddq\t$%d, %s", 8, stackptr_reg(QREG_NAMES));
    }
    for (int i = callee_saved_count - 1; i >= 0; i--) {
        append_code(tail_codes, "\tpopq\t%s\n", callee_reg_name(QREG_NAMES, i));
    }

    sub_codes = gen_inst_x64code(x64gen);
    vector_extend(tail_codes, sub_codes);
    delete_vector(sub_codes, free);

    vector_extend(codes, head_codes);
    delete_vector(head_codes, free);
    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);
    vector_extend(codes, tail_codes);
    delete_vector(tail_codes, free);

    x64gen->callee_saved_count = 0;
    return codes;
}

Vector* gen_function_body_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Vector* sub_codes = NULL;

    while (1) {
        Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
        if (immc->type == IMMC_INST && immc->inst->type == INST_LEAVE) break;

        switch (immc->type) {
            case IMMC_INST:
                sub_codes = gen_inst_x64code(x64gen);
                break;
            case IMMC_LABEL:
                sub_codes = gen_label_x64code(x64gen);
                break;
        }
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    return codes;
}
