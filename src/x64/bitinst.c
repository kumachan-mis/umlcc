#include "./bitinst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_seteq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    char* fst_src_name = LREG_NAMES[fst_src_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            char* snd_src_name = LREG_NAMES[snd_src_id];
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    append_code(codes, "\tsete\t%s\n", BREG_NAMES[dst_id]);
    append_code(codes, "\tmovsbl\t%s, %s\n", BREG_NAMES[dst_id], LREG_NAMES[dst_id]);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_setneq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    char* fst_src_name = LREG_NAMES[fst_src_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            char* snd_src_name = LREG_NAMES[snd_src_id];
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    append_code(codes, "\tsetne\t%s\n", BREG_NAMES[dst_id]);
    append_code(codes, "\tmovsbl\t%s, %s\n", BREG_NAMES[dst_id], LREG_NAMES[dst_id]);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}
