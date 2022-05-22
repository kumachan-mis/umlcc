#include "./datainst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    char* dst_name = LREG_NAMES[dst_id];

    switch (src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tmovl\t$%d, %s\n", src->imm_value, dst_name);
            break;
        case OPERAND_PTR: {
            int src_id = CALLER_SAVED_REG_IDS[src->reg_id];
            char* src_name = QREG_NAMES[src_id];
            append_code(codes, "\tmovl\t(%s), %s\n", src_name, dst_name);
            break;
        }
        case OPERAND_MEM:
            append_code(codes, "\tmovl\t-%d(%s), %s\n", src->mem_offset, BP_NAME, dst_name);
            break;
        case OPERAND_LABEL:
            append_code(codes, "\tmovl\t%s(%s), %s\n", src->label_name, PC_NAME, dst_name);
            break;
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", src->type);
            exit(1);
    }

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_addr_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    char* dst_name = QREG_NAMES[dst_id];

    switch (src->type) {
        case OPERAND_MEM:
            append_code(codes, "\tleaq\t-%d(%s), %s\n", src->mem_offset, BP_NAME, dst_name);
            break;
        case OPERAND_LABEL:
            append_code(codes, "\tleaq\t%s(%s), %s\n", src->label_name, PC_NAME, dst_name);
            break;
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", src->type);
            exit(1);
    }

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_store_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    int src_id = CALLER_SAVED_REG_IDS[src->reg_id];
    char* src_name = LREG_NAMES[src_id];

    switch (dst->type) {
        case OPERAND_PTR: {
            int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
            char* dst_name = QREG_NAMES[dst_id];
            append_code(codes, "\tmovl\t%s, (%s)\n", src_name, dst_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_ldarg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* src = immc->inst->fst_src;

    if (src->imm_value < NUM_ARG_REGS) {
        int src_id = ARG_REG_IDS[src->imm_value];
        char* src_name = LREG_NAMES[src_id];
        append_code(codes, "\tmovl\t%s, -%d(%s)\n", src_name, dst->mem_offset, BP_NAME);
        return codes;
    }

    int reg_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
    char* reg_name = LREG_NAMES[reg_id];
    int mem_arg_offset = (src->imm_value - NUM_ARG_REGS + 1) * 8 + 8;
    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)

    append_code(codes, "\tmovl\t%d(%s), %s\n", mem_arg_offset, BP_NAME, reg_name);
    append_code(codes, "\tmovl\t%s, -%d(%s)\n", reg_name, dst->mem_offset, BP_NAME);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* src = immc->inst->snd_src;

    int src_id = CALLER_SAVED_REG_IDS[src->reg_id];
    char* src_name = QREG_NAMES[src_id];
    append_code(codes, "\tpushq\t%s\n", src_name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_stret_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* src = immc->inst->fst_src;
    int ret_id = CALLER_SAVED_REG_IDS[src->reg_id];
    char* ret_name = LREG_NAMES[ret_id];

    append_code(codes, "\tmovl\t%s, %s\n", ret_name, LREG_NAMES[AX_REG_ID]);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}
