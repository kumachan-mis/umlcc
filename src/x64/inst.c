#include "./inst.h"
#include "../immc/immc.h"
#include "./consts.h"
#include "./regalloc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_load_x64code(X64gen* x64gen);
Vector* gen_addr_x64code(X64gen* x64gen);
Vector* gen_store_x64code(X64gen* x64gen);
Vector* gen_ldarg_x64code(X64gen* x64gen);
Vector* gen_starg_x64code(X64gen* x64gen);
Vector* gen_stret_x64code(X64gen* x64gen);
Vector* gen_add_x64code(X64gen* x64gen);
Vector* gen_sub_x64code(X64gen* x64gen);
Vector* gen_mul_x64code(X64gen* x64gen);
Vector* gen_div_x64code(X64gen* x64gen);
Vector* gen_mod_x64code(X64gen* x64gen);
Vector* gen_seteq_x64code(X64gen* x64gen);
Vector* gen_setneq_x64code(X64gen* x64gen);
Vector* gen_jmp_x64code(X64gen* x64gen);
Vector* gen_jeq_x64code(X64gen* x64gen);
Vector* gen_jneq_x64code(X64gen* x64gen);
Vector* gen_call_x64code(X64gen* x64gen);
Vector* gen_enter_x64code(X64gen* x64gen);
Vector* gen_leave_x64code(X64gen* x64gen);
Vector* gen_free_x64code(X64gen* x64gen);
Vector* gen_prep_x64code(X64gen* x64gen);
Vector* gen_clean_x64code(X64gen* x64gen);

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
        case INST_STRET:
            return gen_stret_x64code(x64gen);
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
        case INST_SETEQ:
            return gen_seteq_x64code(x64gen);
        case INST_SETNEQ:
            return gen_setneq_x64code(x64gen);
        case INST_JMP:
            return gen_jmp_x64code(x64gen);
        case INST_JEQ:
            return gen_jeq_x64code(x64gen);
        case INST_JNEQ:
            return gen_jneq_x64code(x64gen);
        case INST_CALL:
            return gen_call_x64code(x64gen);
        case INST_ENTER:
            return gen_enter_x64code(x64gen);
        case INST_LEAVE:
            return gen_leave_x64code(x64gen);
        case INST_FREE:
            return gen_free_x64code(x64gen);
        case INST_PREP:
            return gen_prep_x64code(x64gen);
        case INST_CLEAN:
            return gen_clean_x64code(x64gen);
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

    int dest_id = regalloc_resolve(x64gen->regalloc, dest->reg_id);
    if (dest_id == -1) dest_id = regalloc_allocate(x64gen->regalloc, dest->reg_id);
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

    int dest_id = regalloc_allocate(x64gen->regalloc, dest->reg_id);
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

    int src_id = regalloc_resolve(x64gen->regalloc, src->reg_id);
    char* src_name = LREG_NAMES[src_id];
    regalloc_free(x64gen->regalloc, src->reg_id);

    switch (dest->type) {
        case OPERAND_PTR: {
            int dest_id = regalloc_resolve(x64gen->regalloc, dest->reg_id);
            char* dest_name = QREG_NAMES[dest_id];
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
        append_code(codes, "\tmovl\t%s, -%d(%s)\n", src_name, dest->mem_offset, BP_NAME);
        return codes;
    }

    int reg_id = regalloc_lock(x64gen->regalloc);
    char* reg_name = LREG_NAMES[reg_id];
    int mem_arg_offset = (src->imm_value - NUM_ARG_REGS + 1) * 8 + 8;
    // (1-indexed non-register param no.) * (bytes of memory address) + (offset for pushq %rbp)

    append_code(codes, "\tmovl\t%d(%s), %s\n", mem_arg_offset, BP_NAME, reg_name);
    append_code(codes, "\tmov\t%s, -%d(%s)\n", reg_name, dest->mem_offset, BP_NAME);
    regalloc_unlock(x64gen->regalloc, reg_id);

    return codes;
}

Vector* gen_starg_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->snd_src;

    int src_id = regalloc_resolve(x64gen->regalloc, src->reg_id);
    char* src_name = QREG_NAMES[src_id];
    append_code(codes, "\tpushq\t%s\n", src_name);

    regalloc_free(x64gen->regalloc, src->reg_id);
    return codes;
}

Vector* gen_stret_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int ret_id = regalloc_resolve(x64gen->regalloc, src->reg_id);
    char* ret_name = LREG_NAMES[ret_id];

    append_code(codes, "\tmovl\t%s, %s\n", ret_name, LREG_NAMES[AX_REG_ID]);

    return codes;
}

Vector* gen_add_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
    regalloc_free(x64gen->regalloc, fst_src->reg_id);
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

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
    regalloc_free(x64gen->regalloc, fst_src->reg_id);
    regalloc_free(x64gen->regalloc, snd_src->reg_id);

    int dest_id = regalloc_force_allocate(x64gen->regalloc, dest->reg_id, fst_src_id);

    char* src_name = LREG_NAMES[snd_src_id];
    char* dest_name = LREG_NAMES[dest_id];
    append_code(codes, "\tsubl\t%s, %s\n", src_name, dest_name);

    return codes;
}

Vector* gen_mul_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
    regalloc_free(x64gen->regalloc, fst_src->reg_id);
    regalloc_free(x64gen->regalloc, snd_src->reg_id);

    int dest_id = regalloc_force_allocate(x64gen->regalloc, dest->reg_id, fst_src_id);

    char* src_name = LREG_NAMES[snd_src_id];
    char* dest_name = LREG_NAMES[dest_id];
    append_code(codes, "\timull\t%s, %s\n", src_name, dest_name);

    return codes;
}

Vector* gen_div_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);

    if (snd_src_id == DX_REG_ID) {
        snd_src_id = regalloc_realloc(x64gen->regalloc, snd_src->reg_id);
        append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[DX_REG_ID], LREG_NAMES[snd_src_id]);
    }

    regalloc_free(x64gen->regalloc, snd_src->reg_id);
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    int dest_id = regalloc_force_allocate(x64gen->regalloc, dest->reg_id, fst_src_id);

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dest_name = LREG_NAMES[dest_id];

    append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, LREG_NAMES[AX_REG_ID]);
    append_code(codes, "\tcltd\n");
    append_code(codes, "\tidivl\t%s\n", snd_src_name);
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[AX_REG_ID], dest_name);

    return codes;
}

Vector* gen_mod_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);

    if (snd_src_id == DX_REG_ID) {
        snd_src_id = regalloc_realloc(x64gen->regalloc, snd_src->reg_id);
        append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[DX_REG_ID], LREG_NAMES[snd_src_id]);
    }

    regalloc_free(x64gen->regalloc, fst_src->reg_id);
    regalloc_free(x64gen->regalloc, snd_src->reg_id);

    int dest_id = regalloc_force_allocate(x64gen->regalloc, dest->reg_id, fst_src_id);

    char* fst_src_name = LREG_NAMES[fst_src_id];
    char* snd_src_name = LREG_NAMES[snd_src_id];
    char* dest_name = LREG_NAMES[dest_id];

    append_code(codes, "\tmovl\t%s, %s\n", fst_src_name, LREG_NAMES[AX_REG_ID]);
    append_code(codes, " cltd\n");
    append_code(codes, "\tidivl\t%s\n", snd_src_name);
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[DX_REG_ID], dest_name);

    return codes;
}

Vector* gen_seteq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    char* fst_src_name = LREG_NAMES[fst_src_id];
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
            char* snd_src_name = LREG_NAMES[snd_src_id];
            regalloc_free(x64gen->regalloc, snd_src->reg_id);
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }

    int dest_id = regalloc_allocate(x64gen->regalloc, dest->reg_id);
    append_code(codes, "\tsete\t%s\n", BREG_NAMES[dest_id]);
    append_code(codes, "\tmovsbl\t%s, %s\n", BREG_NAMES[dest_id], LREG_NAMES[dest_id]);

    return codes;
}

Vector* gen_setneq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    char* fst_src_name = LREG_NAMES[fst_src_id];
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
            char* snd_src_name = LREG_NAMES[snd_src_id];
            regalloc_free(x64gen->regalloc, snd_src->reg_id);
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }

    int dest_id = regalloc_allocate(x64gen->regalloc, dest->reg_id);
    append_code(codes, "\tsetne\t%s\n", BREG_NAMES[dest_id]);
    append_code(codes, "\tmovsbl\t%s, %s\n", BREG_NAMES[dest_id], LREG_NAMES[dest_id]);

    return codes;
}

Vector* gen_jeq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    char* fst_src_name = LREG_NAMES[fst_src_id];
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
            char* snd_src_name = LREG_NAMES[snd_src_id];
            regalloc_free(x64gen->regalloc, snd_src->reg_id);
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }

    append_code(codes, "\tje\t%s\n", dest->label_name);

    return codes;
}

Vector* gen_jneq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    char* fst_src_name = LREG_NAMES[fst_src_id];
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = regalloc_resolve(x64gen->regalloc, snd_src->reg_id);
            char* snd_src_name = LREG_NAMES[snd_src_id];
            regalloc_free(x64gen->regalloc, snd_src->reg_id);
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }

    append_code(codes, "\tjne\t%s\n", dest->label_name);

    return codes;
}

Vector* gen_jmp_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;

    append_code(codes, "\tjmp\t%s\n", dest->label_name);

    return codes;
}

Vector* gen_call_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    for (int i = 0; i < snd_src->imm_value && i < NUM_ARG_REGS; i++) {
        int arg_id = ARG_REG_IDS[i];
        char* arg_name = QREG_NAMES[arg_id];

        int using_virtual_id = regalloc_usedby(x64gen->regalloc, arg_id);
        if (using_virtual_id != -1) {
            int evacuation_id = regalloc_realloc(x64gen->regalloc, using_virtual_id);
            char* evacuation_name = QREG_NAMES[evacuation_id];
            append_code(codes, "\tmovq\t%s, %s\n", arg_name, evacuation_name);
        }

        regalloc_force_lock(x64gen->regalloc, arg_id);
        append_code(codes, "\tpopq\t%s\n", arg_name);
    }

    int src_id = regalloc_resolve(x64gen->regalloc, fst_src->reg_id);
    char* src_name = QREG_NAMES[src_id];
    regalloc_free(x64gen->regalloc, fst_src->reg_id);

    for (int i = 0; i < snd_src->imm_value && i < NUM_ARG_REGS; i++) {
        regalloc_unlock(x64gen->regalloc, ARG_REG_IDS[i]);
    }

    Vector* evacuation_table = regalloc_evacuate(x64gen->regalloc);
    int evaluation_count = vector_size(evacuation_table);
    for (int i = 0; i < evaluation_count; i++) {
        RegEvacuationEntry* entry = vector_at(evacuation_table, i);
        char* caller_saved_name = QREG_NAMES[CALLER_SAVED_REG_IDS[entry->caller_saved_index]];
        char* callee_saved_name = QREG_NAMES[CALLEE_SAVED_REG_IDS[entry->callee_saved_index]];
        append_code(codes, "\tmovq\t%s, %s\n", caller_saved_name, callee_saved_name);
    }

    append_code(codes, "\tmovl\t$%d, %s\n", 0, LREG_NAMES[AX_REG_ID]);
    append_code(codes, "\tcall\t*%s\n", src_name);

    for (int i = 0; i < evaluation_count; i++) {
        RegEvacuationEntry* entry = vector_at(evacuation_table, i);
        char* caller_saved_name = QREG_NAMES[CALLER_SAVED_REG_IDS[entry->caller_saved_index]];
        char* callee_saved_name = QREG_NAMES[CALLEE_SAVED_REG_IDS[entry->callee_saved_index]];
        append_code(codes, "\tmovq\t%s, %s\n", callee_saved_name, caller_saved_name);
    }
    if (x64gen->evaluation_count < evaluation_count) x64gen->evaluation_count = evaluation_count;
    regalloc_restore(x64gen->regalloc, evacuation_table);

    int dest_id = regalloc_allocate(x64gen->regalloc, dest->reg_id);
    char* dest_name = LREG_NAMES[dest_id];
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[AX_REG_ID], dest_name);

    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\tpushq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[SP_REG_ID], QREG_NAMES[BP_REG_ID]);
    if (aligned_memory_size > 0) {
        append_code(codes, "\tsubq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    }

    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    if (aligned_memory_size) {
        append_code(codes, "\taddq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    }
    append_code(codes, "\tpopq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tret\n");

    return codes;
}

Vector* gen_prep_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* fst_src = immc->inst->fst_src;

    if (fst_src->imm_value > NUM_ARG_REGS && fst_src->imm_value % 2 == 1) {
        append_code(codes, "\tsubq\t$%d, %s\n", 8, QREG_NAMES[SP_REG_ID]);
    }

    return codes;
}

Vector* gen_clean_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* fst_src = immc->inst->fst_src;

    if (fst_src->imm_value > NUM_ARG_REGS) {
        int mem_param_offset = ((fst_src->imm_value - NUM_ARG_REGS + 1) / 2) * 16;
        append_code(codes, "\taddq\t$%d, %s\n", mem_param_offset, QREG_NAMES[SP_REG_ID]);
    }

    return codes;
}

Vector* gen_free_x64code(X64gen* x64gen) {
    Vector* codes = new_vector();
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    regalloc_free(x64gen->regalloc, src->reg_id);

    return codes;
}
