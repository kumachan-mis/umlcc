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

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];

    ImmcSuffix immc_suffix = IMMC_SUFFIX_NONE;
    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            immc_suffix = fst_src->suffix;
            append_mov_code(codes, fst_src_id, fst_src->suffix, fst_src_id, immc_suffix);
            char* fst_src_name = reg_name(fst_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\tadd%c\t$%d, %s\n", suffix, snd_src->imm_value, fst_src_name);
            break;
        }
        case IMMC_OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            immc_suffix = immcsuffix_greater(fst_src->suffix, snd_src->suffix);
            append_mov_code(codes, fst_src_id, fst_src->suffix, fst_src_id, immc_suffix);
            append_mov_code(codes, snd_src_id, snd_src->suffix, snd_src_id, immc_suffix);
            char* fst_src_name = reg_name(fst_src_id, immc_suffix);
            char* snd_src_name = reg_name(snd_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\tadd%c\t%s, %s\n", suffix, snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    append_mov_code(codes, fst_src_id, immc_suffix, dst_id, dst->suffix);
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

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];

    ImmcSuffix immc_suffix = IMMC_SUFFIX_NONE;
    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            immc_suffix = fst_src->suffix;
            append_mov_code(codes, fst_src_id, fst_src->suffix, fst_src_id, immc_suffix);
            char* fst_src_name = reg_name(fst_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\tsub%c\t$%d, %s\n", suffix, snd_src->imm_value, fst_src_name);
            break;
        }
        case IMMC_OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            immc_suffix = immcsuffix_greater(fst_src->suffix, snd_src->suffix);
            append_mov_code(codes, fst_src_id, fst_src->suffix, fst_src_id, immc_suffix);
            append_mov_code(codes, snd_src_id, snd_src->suffix, snd_src_id, immc_suffix);
            char* fst_src_name = reg_name(fst_src_id, immc_suffix);
            char* snd_src_name = reg_name(snd_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\tsub%c\t%s, %s\n", suffix, snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    append_mov_code(codes, fst_src_id, immc_suffix, dst_id, dst->suffix);
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

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];

    ImmcSuffix immc_suffix = IMMC_SUFFIX_NONE;
    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            immc_suffix = fst_src->suffix;
            append_mov_code(codes, fst_src_id, fst_src->suffix, AX_REG_ID, immc_suffix);
            char* fst_src_name = reg_name(AX_REG_ID, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\timul%c\t$%d, %s\n", suffix, snd_src->imm_value, fst_src_name);
            break;
        }
        case IMMC_OPERAND_REG: {
            immc_suffix = immcsuffix_greater(fst_src->suffix, snd_src->suffix);
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            append_mov_code(codes, fst_src_id, fst_src->suffix, AX_REG_ID, immc_suffix);
            append_mov_code(codes, snd_src_id, snd_src->suffix, snd_src_id, immc_suffix);
            char* fst_src_name = reg_name(AX_REG_ID, immc_suffix);
            char* snd_src_name = reg_name(snd_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\timul%c\t%s, %s\n", suffix, snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    append_mov_code(codes, AX_REG_ID, immc_suffix, dst_id, dst->suffix);
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

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = -1;

    ImmcSuffix immc_suffix = IMMC_SUFFIX_NONE;
    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            snd_src_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
            immc_suffix = fst_src->suffix;
            append_mov_code(codes, fst_src_id, fst_src->suffix, AX_REG_ID, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            char* snd_src_name = reg_name(snd_src_id, immc_suffix);
            append_code(codes, "\tmov%c\t$%d, %s\n", suffix, snd_src->imm_value, snd_src_name);
            break;
        }
        case IMMC_OPERAND_REG: {
            snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            immc_suffix = immcsuffix_greater(fst_src->suffix, snd_src->suffix);
            append_mov_code(codes, fst_src_id, fst_src->suffix, AX_REG_ID, immc_suffix);
            if (snd_src_id != DX_REG_ID) {
                append_mov_code(codes, snd_src_id, snd_src->suffix, snd_src_id, immc_suffix);
            } else {
                append_mov_code(codes, snd_src_id, snd_src->suffix, fst_src_id, immc_suffix);
                snd_src_id = fst_src_id;
            }
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    char* snd_src_name = reg_name(snd_src_id, immc_suffix);
    char suffix = immcsuffix_tochar(immc_suffix);

    append_code(codes, "\tc%ctd\n", suffix);
    append_code(codes, "\tidiv%c\t%s\n", suffix, snd_src_name);
    append_mov_code(codes, AX_REG_ID, immc_suffix, dst_id, dst->suffix);

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

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int snd_src_id = -1;

    ImmcSuffix immc_suffix = IMMC_SUFFIX_NONE;
    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            snd_src_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
            immc_suffix = fst_src->suffix;
            append_mov_code(codes, fst_src_id, fst_src->suffix, AX_REG_ID, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            char* snd_src_name = reg_name(snd_src_id, immc_suffix);
            append_code(codes, "\tmov%c\t$%d, %s\n", suffix, snd_src->imm_value, snd_src_name);
            break;
        }
        case IMMC_OPERAND_REG: {
            snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            immc_suffix = immcsuffix_greater(fst_src->suffix, snd_src->suffix);
            append_mov_code(codes, fst_src_id, fst_src->suffix, AX_REG_ID, immc_suffix);
            if (snd_src_id != DX_REG_ID) {
                append_mov_code(codes, snd_src_id, snd_src->suffix, snd_src_id, immc_suffix);
            } else {
                append_mov_code(codes, snd_src_id, snd_src->suffix, fst_src_id, immc_suffix);
                snd_src_id = fst_src_id;
            }
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    char* snd_src_name = reg_name(snd_src_id, immc_suffix);
    char suffix = immcsuffix_tochar(immc_suffix);

    append_code(codes, "\tc%ctd\n", suffix);
    append_code(codes, "\tidiv%c\t%s\n", suffix, snd_src_name);
    append_mov_code(codes, DX_REG_ID, immc_suffix, dst_id, dst->suffix);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
