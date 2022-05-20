#include "./datainst.h"
#include "../immc/immc.h"
#include "./consts.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int dest_id = -1;
    char* dest_name = LREG_NAMES[dest_id];

    switch (src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tmovl\t$%d, %s\n", src->imm_value, dest_name);
            break;
        case OPERAND_MEM:
            append_code(codes, "\tmovl\t-%d(%s), %s\n", src->mem_offset, BP_NAME, dest_name);
            break;
        case OPERAND_LABEL:
            append_code(codes, "\tmovl\t%s(%s), %s\n", src->label_name, PC_NAME, dest_name);
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
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int dest_id = -1;
    char* dest_name = QREG_NAMES[dest_id];

    switch (src->type) {
        case OPERAND_MEM:
            append_code(codes, "\tleaq\t-%d(%s), %s\n", src->mem_offset, BP_NAME, dest_name);
            break;
        case OPERAND_LABEL:
            append_code(codes, "\tleaq\t%s(%s), %s\n", src->label_name, PC_NAME, dest_name);
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

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int src_id = -1;
    char* src_name = LREG_NAMES[src_id];

    switch (dest->type) {
        case OPERAND_PTR: {
            int dest_id = -1;
            char* dest_name = QREG_NAMES[dest_id];
            append_code(codes, "\tmovl\t%s, (%s)\n", src_name, dest_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
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
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    if (src->imm_value < NUM_ARG_REGS) {
        int src_id = ARG_REG_IDS[src->imm_value];
        char* src_name = LREG_NAMES[src_id];
        append_code(codes, "\tmovl\t%s, -%d(%s)\n", src_name, dest->mem_offset, BP_NAME);
        return codes;
    }

    int reg_id = -1;
    char* reg_name = LREG_NAMES[reg_id];
    int mem_arg_offset = (src->imm_value - NUM_ARG_REGS + 1) * 8 + 8;
    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)

    append_code(codes, "\tmovl\t%d(%s), %s\n", mem_arg_offset, BP_NAME, reg_name);
    append_code(codes, "\tmov\t%s, -%d(%s)\n", reg_name, dest->mem_offset, BP_NAME);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* src = immc->inst->snd_src;

    int src_id = -1;
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
    int ret_id = -1;
    char* ret_name = LREG_NAMES[ret_id];

    append_code(codes, "\tmovl\t%s, %s\n", ret_name, LREG_NAMES[AX_REG_ID]);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}
