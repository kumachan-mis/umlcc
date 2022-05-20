#include "./ctrlinst.h"
#include "../immc/immc.h"
#include "./consts.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_jeq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = -1;
    char* fst_src_name = LREG_NAMES[fst_src_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = -1;
            char* snd_src_name = LREG_NAMES[snd_src_id];
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }

    append_code(codes, "\tje\t%s\n", dest->label_name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_jneq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = -1;
    char* fst_src_name = LREG_NAMES[fst_src_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = -1;
            char* snd_src_name = LREG_NAMES[snd_src_id];
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dest->type);
            exit(1);
    }

    append_code(codes, "\tjne\t%s\n", dest->label_name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_jmp_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dest = immc->inst->dest;

    append_code(codes, "\tjmp\t%s\n", dest->label_name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_call_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* dest = immc->inst->dest;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int src_id = -1;
    char* src_name = QREG_NAMES[src_id];

    append_code(codes, "\tmovl\t$%d, %s\n", 0, LREG_NAMES[AX_REG_ID]);
    append_code(codes, "\tcall\t*%s\n", src_name);

    int dest_id = -1;
    char* dest_name = LREG_NAMES[dest_id];
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[AX_REG_ID], dest_name);

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\tpushq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[SP_REG_ID], QREG_NAMES[BP_REG_ID]);
    if (aligned_memory_size > 0) {
        append_code(codes, "\tsubq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    }

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    if (aligned_memory_size) {
        append_code(codes, "\taddq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    }
    append_code(codes, "\tpopq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tret\n");

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_prep_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* fst_src = immc->inst->fst_src;

    if (fst_src->imm_value > NUM_ARG_REGS && fst_src->imm_value % 2 == 1) {
        append_code(codes, "\tsubq\t$%d, %s\n", 8, QREG_NAMES[SP_REG_ID]);
    }

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}

Vector* gen_clean_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->_immcs, x64gen->_index);
    x64gen->_index++;

    ImmcOpe* fst_src = immc->inst->fst_src;

    if (fst_src->imm_value > NUM_ARG_REGS) {
        int mem_param_offset = ((fst_src->imm_value - NUM_ARG_REGS + 1) / 2) * 16;
        append_code(codes, "\taddq\t$%d, %s\n", mem_param_offset, QREG_NAMES[SP_REG_ID]);
    }

    liveseqs_next(x64gen->_liveseqs);
    return codes;
}
