#include "./arithinst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_add_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    append_code(codes, "\taddl\t%s, %s\n", snd_src_name, fst_src_name);
    if (dst_id != fst_src_id) append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_sub_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    append_code(codes, "\tsubl\t%s, %s\n", snd_src_name, fst_src_name);
    if (dst_id != fst_src_id) append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_mul_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, LREG_NAMES[AX_REG_ID]);
    append_code(codes, "\timull\t%s, %s\n", snd_src_name, LREG_NAMES[AX_REG_ID]);
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[AX_REG_ID], dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_div_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, LREG_NAMES[AX_REG_ID]);
    if (snd_src_id == DX_REG_ID) {
        append_code(codes, "\tmovl\t%s, %s\n", snd_src_name, fst_src_name);
        snd_src_name = fst_src_name;
    }

    append_code(codes, "\tcltd\n");
    append_code(codes, "\tidivl\t%s\n", snd_src_name);
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[AX_REG_ID], dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_mod_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dst_name = LREG_NAMES[dst_id];

    append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, LREG_NAMES[AX_REG_ID]);
    if (snd_src_id == DX_REG_ID) {
        append_code(codes, "\tmovl\t%s, %s\n", snd_src_name, fst_src_name);
        snd_src_name = fst_src_name;
    }

    append_code(codes, " cltd\n");
    append_code(codes, "\tidivl\t%s\n", snd_src_name);
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[DX_REG_ID], dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
