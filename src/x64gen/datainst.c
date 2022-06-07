#include "./datainst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    char* dst_name = reg_name(dst_id, dst->suffix);
    char suffix = immcsuffix_tochar(dst->suffix);

    switch (src->type) {
        case IMMC_OPERAND_IMM: {
            int imm_value = src->imm_value;
            append_code(codes, "\tmov%c\t$%d, %s\n", suffix, imm_value, dst_name);
            break;
        }
        case IMMC_OPERAND_PTR: {
            int src_id = CALLER_SAVED_REG_IDS[src->reg_id];
            char* src_name = QREG_NAMES[src_id];
            append_code(codes, "\tmov%c\t(%s), %s\n", suffix, src_name, dst_name);
            break;
        }
        case IMMC_OPERAND_MEM: {
            int mem_offset = src->mem_offset;
            append_code(codes, "\tmov%c\t-%d(%s), %s\n", suffix, mem_offset, BP_NAME, dst_name);
            break;
        }
        case IMMC_OPERAND_LABEL: {
            char* label_name = src->label_name;
            append_code(codes, "\tmov%c\t%s(%s), %s\n", suffix, label_name, PC_NAME, dst_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_addr_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    char* dst_name = QREG_NAMES[dst_id];

    switch (src->type) {
        case IMMC_OPERAND_MEM:
            append_code(codes, "\tleaq\t-%d(%s), %s\n", src->mem_offset, BP_NAME, dst_name);
            break;
        case IMMC_OPERAND_LABEL:
            append_code(codes, "\tleaq\t%s(%s), %s\n", src->label_name, PC_NAME, dst_name);
            break;
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_store_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    int src_id = CALLER_SAVED_REG_IDS[src->reg_id];
    char* src_name = reg_name(src_id, src->suffix);
    char suffix = immcsuffix_tochar(src->suffix);

    switch (dst->type) {
        case IMMC_OPERAND_PTR: {
            int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
            char* dst_name = QREG_NAMES[dst_id];
            append_code(codes, "\tmov%c\t%s, (%s)\n", suffix, src_name, dst_name);
            break;
        }
        case IMMC_OPERAND_MEM: {
            int mem_offset = dst->mem_offset;
            append_code(codes, "\tmov%c\t%s, -%d(%s)\n", suffix, src_name, mem_offset, BP_NAME);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_ldarg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    if (src->imm_value < NUM_ARG_REGS) {
        int src_id = ARG_REG_IDS[src->imm_value];
        char* src_name = reg_name(src_id, src->suffix);
        char suffix = immcsuffix_tochar(src->suffix);
        append_code(codes, "\tmov%c\t%s, -%d(%s)\n", suffix, src_name, dst->mem_offset, BP_NAME);
        return codes;
    }

    int tmp_reg_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
    char* tmp_reg_name = reg_name(tmp_reg_id, src->suffix);
    char suffix = immcsuffix_tochar(src->suffix);

    int mem_arg_offset = (src->imm_value - NUM_ARG_REGS + 1) * 8 + 8;
    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)

    append_code(codes, "\tmov%c\t%d(%s), %s\n", suffix, mem_arg_offset, BP_NAME, tmp_reg_name);
    append_code(codes, "\tmov%c\t%s, -%d(%s)\n", suffix, tmp_reg_name, dst->mem_offset, BP_NAME);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->snd_src;

    int src_id = CALLER_SAVED_REG_IDS[src->reg_id];
    char* src_name = QREG_NAMES[src_id];
    append_code(codes, "\tpushq\t%s\n", src_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_stret_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* ret = immc->inst->fst_src;
    int ret_id = CALLER_SAVED_REG_IDS[ret->reg_id];
    append_mov_code(codes, ret_id, ret->suffix, AX_REG_ID, ret->suffix);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
