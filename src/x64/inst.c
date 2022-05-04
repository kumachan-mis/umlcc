#include "./inst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

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

    char* base_ptr = baseptr_reg(quad_regs);
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int dest_name = caller_saved_map_allocate(x64gen->caller_saved_map, dest->reg_name);
    char* dest_reg = caller_saved_reg(double_regs, dest_name);

    switch (src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tmovl\t$%d, %s\n", src->imm_value, dest_reg);
            break;
        case OPERAND_MEM:
            append_code(codes, "\tmovl\t-%d(%s), %s\n", src->mem_offset, base_ptr, dest_reg);
            break;
        case OPERAND_LABEL:
            append_code(codes, "\tmovl\t%s(%s), %s\n", src->label_name, prog_counter, dest_reg);
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

    char* base_ptr = baseptr_reg(quad_regs);
    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* src = immc->inst->fst_src;

    int dest_name = caller_saved_map_allocate(x64gen->caller_saved_map, dest->reg_name);
    char* dest_reg = caller_saved_reg(quad_regs, dest_name);

    switch (src->type) {
        case OPERAND_MEM:
            append_code(codes, "\tleaq\t-%d(%s), %s\n", src->mem_offset, base_ptr, dest_reg);
            break;
        case OPERAND_LABEL:
            append_code(codes, "\tleaq\t%s(%s), %s\n", src->label_name, prog_counter, dest_reg);
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

    int src_name = caller_saved_map_search(x64gen->caller_saved_map, src->reg_name);
    char* src_reg = caller_saved_reg(double_regs, src_name);
    caller_saved_map_free(x64gen->caller_saved_map, src->reg_name);

    switch (dest->type) {
        case OPERAND_REG: {
            int dest_name = caller_saved_map_allocate(x64gen->caller_saved_map, dest->reg_name);
            char* dest_reg = caller_saved_reg(double_regs, dest_name);
            append_code(codes, "\tmovl\t%s, %s\n", src_reg, dest_reg);
            break;
        }
        case OPERAND_PTR: {
            int dest_name = caller_saved_map_search(x64gen->caller_saved_map, dest->reg_name);
            char* dest_reg = caller_saved_reg(quad_regs, dest_name);
            caller_saved_map_free(x64gen->caller_saved_map, dest->reg_name);
            append_code(codes, "\tmovl\t%s, (%s)\n", src_reg, dest_reg);
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
    // Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    // Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    return codes;
}

Vector* gen_add_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_name = caller_saved_map_search(x64gen->caller_saved_map, fst_src->reg_name);
    char* fst_src_reg = caller_saved_reg(double_regs, fst_src_name);
    caller_saved_map_force_allocate(x64gen->caller_saved_map, fst_src_name, dest->reg_name);

    int snd_src_name = caller_saved_map_search(x64gen->caller_saved_map, snd_src->reg_name);
    char* snd_src_reg = caller_saved_reg(double_regs, snd_src_name);
    caller_saved_map_free(x64gen->caller_saved_map, snd_src->reg_name);

    append_code(codes, "\taddl\t%s, %s\n", snd_src_reg, fst_src_reg);

    return codes;
}

Vector* gen_sub_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_name = caller_saved_map_search(x64gen->caller_saved_map, fst_src->reg_name);
    char* fst_src_reg = caller_saved_reg(double_regs, fst_src_name);
    caller_saved_map_force_allocate(x64gen->caller_saved_map, fst_src_name, dest->reg_name);

    int snd_src_name = caller_saved_map_search(x64gen->caller_saved_map, snd_src->reg_name);
    char* snd_src_reg = caller_saved_reg(double_regs, snd_src_name);
    caller_saved_map_free(x64gen->caller_saved_map, snd_src->reg_name);

    append_code(codes, "\tsubl\t%s, %s\n", snd_src_reg, fst_src_reg);

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

    // ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    // ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_name = caller_saved_map_search(x64gen->caller_saved_map, fst_src->reg_name);
    char* fst_src_reg = caller_saved_reg(quad_regs, fst_src_name);
    caller_saved_map_free(x64gen->caller_saved_map, fst_src->reg_name);

    append_code(codes, "\tcall\t*%s\n", fst_src_reg);

    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\tpushq\t%s\n", baseptr_reg(quad_regs));
    append_code(codes, "\tsubq\t%d, %s\n", aligned_memory_size, stkptr_reg(quad_regs));

    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\taddq\t%d, %s\n", aligned_memory_size, stkptr_reg(quad_regs));
    append_code(codes, "\tpopq\t%s\n", baseptr_reg(quad_regs));

    return codes;
}
