#include "./inst.h"
#include "../immc/immc.h"
#include "./consts.h"
#include "./util.h"
#include "./regalloc.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen);
Vector* gen_addr_x64code(X64gen* x64gen);
Vector* gen_store_x64code(X64gen* x64gen);
Vector* gen_ldarg_x64code(X64gen* x64gen);
Vector* gen_starg_x64code(X64gen* x64gen);
Vector* gen_add_x64code(X64gen* x64gen);
Vector* gen_sub_x64code(X64gen* x64gen);
Vector* gen_mul_x64code(X64gen* x64gen);
Vector* gen_div_x64code(X64gen* x64gen);
Vector* gen_mod_x64code(X64gen* x64gen);
Vector* gen_call_x64code(X64gen* x64gen);
Vector* gen_enter_x64code(X64gen* x64gen);
Vector* gen_leave_x64code(X64gen* x64gen);

Vector* gen_inst_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    switch (immc->inst->type) {
        case INST_LOAD:
            return gen_load_x64code(x64gen);
        case INST_ADDR:
            return gen_addr_x64code(x64gen);
        case INST_STORE:
            return gen_store_x64code(x64gen);
        case INST_LDARG:
            return gen_ldarg_x64code(x64gen);
        case INST_STARG:
            return gen_starg_x64code(x64gen);
        case INST_ADD:
            return gen_add_x64code(x64gen);
        case INST_SUB:
            return gen_sub_x64code(x64gen);
        case INST_MUL:
            return gen_mul_x64code(x64gen);
        case INST_DIV:
            return gen_div_x64code(x64gen);
        case INST_MOD:
            return gen_mod_x64code(x64gen);
        case INST_CALL:
            return gen_call_x64code(x64gen);
        case INST_ENTER:
            return gen_enter_x64code(x64gen);
        case INST_LEAVE:
            return gen_leave_x64code(x64gen);
        default:
            fprintf(stderr, "Error: unexpected imcc inst %d\n", immc->inst->type);
            exit(1);
    }
}

Vector* gen_load_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int dest_id = regalloc_allocate_caller_saved(x64gen->regalloc, dest->reg_id);
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

    return codes;
}

Vector* gen_addr_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int dest_id = regalloc_allocate_caller_saved(x64gen->regalloc, dest->reg_id);
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

    return codes;
}

Vector* gen_store_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int src_id = regalloc_search(x64gen->regalloc, src->reg_id);
    char* src_name = LREG_NAMES[src_id];
    regalloc_free(x64gen->regalloc, src->reg_id);

    switch (dest->type) {
        case OPERAND_REG: {
            int dest_id = regalloc_allocate_caller_saved(x64gen->regalloc, dest->reg_id);
            char* dest_name = LREG_NAMES[dest_id];
            append_code(codes, "\tmovl\t%s, %s\n", src_name, dest_name);
            break;
        }
        case OPERAND_PTR: {
            int dest_id = regalloc_search(x64gen->regalloc, dest->reg_id);
            char* dest_name = LREG_NAMES[dest_id];
            regalloc_free(x64gen->regalloc, dest->reg_id);
            append_code(codes, "\tmovl\t%s, (%s)\n", src_name, dest_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }
    return codes;
}

Vector* gen_ldarg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    char* BP_NAME = QREG_NAMES[BP_REG_ID];
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    if (src->imm_value < NUM_ARG_REGS) {
        int src_id = ARG_REG_IDS[src->imm_value];
        char* src_name = LREG_NAMES[src_id];
        append_code(codes, "\tmov\t%s, -%d(%s)\n", src_name, dest->mem_offset, BP_NAME);
        return codes;
    }

    int reg_id = regalloc_allocate_caller_saved(x64gen->regalloc, 0);
    char* reg_name = LREG_NAMES[reg_id];
    int mem_arg_offset = (src->imm_value - NUM_ARG_REGS + 1) * 8 + 8;
    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)

    append_code(codes, "\tmov\t-%d(%s), %s\n", dest->mem_offset, BP_NAME, reg_name);
    append_code(codes, "\tmov\t%s, -%d(%s)\n", reg_name, mem_arg_offset, BP_NAME);
    regalloc_free(x64gen->regalloc, 0);

    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->snd_src;

    int src_id = regalloc_search(x64gen->regalloc, src->reg_id);
    regalloc_free(x64gen->regalloc, src->reg_id);

    char* src_name = QREG_NAMES[src_id];
    append_code(codes, "\tpushq\t%s\n", src_name);

    return codes;
}

Vector* gen_add_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_search(x64gen->regalloc, fst_src->reg_id);
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    int snd_src_id = regalloc_search(x64gen->regalloc, snd_src->reg_id);
    regalloc_free(x64gen->regalloc, snd_src->reg_id);

    int dest_id = regalloc_force_allocate(x64gen->regalloc, dest->reg_id, fst_src_id);
    
    char* src_name = LREG_NAMES[snd_src_id];
    char* dest_name = LREG_NAMES[dest_id];
    append_code(codes, "\taddl\t%s, %s\n", src_name, dest_name);

    return codes;
}

Vector* gen_sub_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_search(x64gen->regalloc, fst_src->reg_id);
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    int snd_src_id = regalloc_search(x64gen->regalloc, snd_src->reg_id);
    regalloc_free(x64gen->regalloc, snd_src->reg_id);

    int dest_id = regalloc_force_allocate(x64gen->regalloc, dest->reg_id, fst_src_id);
    
    char* src_name = LREG_NAMES[snd_src_id];
    char* dest_name = LREG_NAMES[dest_id];
    append_code(codes, "\tsubl\t%s, %s\n", src_name, dest_name);

    return codes;
}

Vector* gen_mul_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    // Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    return codes;
}

Vector* gen_div_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    // Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    return codes;
}

Vector* gen_mod_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    // Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    return codes;
}

Vector* gen_call_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int src_id = regalloc_search(x64gen->regalloc, fst_src->reg_id);
    char* src_name = QREG_NAMES[src_id];
    regalloc_free(x64gen->regalloc, fst_src->reg_id);
    append_code(codes, "\tmovq\t%s, %s\n", src_name, QREG_NAMES[AX_REG_ID]);

    for (int i = 0; i < NUM_ARG_REGS && i < snd_src->imm_value; i++) {
        int arg_id = ARG_REG_IDS[i];
        char* arg_name = QREG_NAMES[arg_id];
        append_code(codes, "\tpopq\t%s\n", arg_name);
    }

    append_code(codes, "\tcall\t*%s\n", QREG_NAMES[AX_REG_ID]);
    regalloc_force_allocate(x64gen->regalloc, dest->reg_id, AX_REG_ID);

    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\tpushq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tsubq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);

    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\taddq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    append_code(codes, "\tpopq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tret\n");

    return codes;
}
