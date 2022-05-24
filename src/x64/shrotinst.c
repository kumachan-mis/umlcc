#include "./shrotinst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_sal_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tsall\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    if (dst_id != fst_src_id) append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_sar_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tsarl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    if (dst_id != fst_src_id) append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
