#include "./ctrlinst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_jeq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    char* fst_src_name = LREG_NAMES[fst_src_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            char* snd_src_name = LREG_NAMES[snd_src_id];
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    append_code(codes, "\tje\t%s\n", dst->label_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_jneq_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    char* fst_src_name = LREG_NAMES[fst_src_id];

    switch (snd_src->type) {
        case OPERAND_IMM:
            append_code(codes, "\tcmpl\t$%d, %s\n", snd_src->imm_value, fst_src_name);
            break;
        case OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            char* snd_src_name = LREG_NAMES[snd_src_id];
            append_code(codes, "\tcmpl\t%s, %s\n", snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    append_code(codes, "\tjne\t%s\n", dst->label_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_jmp_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;

    append_code(codes, "\tjmp\t%s\n", dst->label_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_call_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    Set* alive_regs_set = create_alive_regs_set(x64gen->liveseqs);
    int evacuation_count = 0;
    // TODO: consider case that evacuation_count is greater than NUM_CALLEE_SAVED_REG
    for (SetIter* iter = set_iter_begin(alive_regs_set); !set_iter_end(iter, alive_regs_set);
         iter = set_iter_next(iter, alive_regs_set)) {
        int* caller_ref = set_iter_item(iter, alive_regs_set);
        int caller_id = CALLER_SAVED_REG_IDS[*caller_ref];
        int callee_id = CALLEE_SAVED_REG_IDS[evacuation_count];
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[caller_id], QREG_NAMES[callee_id]);
        evacuation_count++;
    }

    int src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
    int evacuation_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
    if (snd_src->imm_value < NUM_ARG_REGS) evacuation_id = ARG_REG_IDS[snd_src->imm_value];
    if (src_id != evacuation_id) {
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[src_id], QREG_NAMES[evacuation_id]);
        src_id = evacuation_id;
    }

    for (int i = 0; i < snd_src->imm_value && i < NUM_ARG_REGS; i++) {
        int arg_id = ARG_REG_IDS[i];
        char* arg_name = QREG_NAMES[arg_id];
        append_code(codes, "\tpopq\t%s\n", arg_name);
    }

    char* src_name = QREG_NAMES[src_id];
    append_code(codes, "\tmovl\t$%d, %s\n", 0, LREG_NAMES[AX_REG_ID]);
    append_code(codes, "\tcall\t*%s\n", src_name);

    evacuation_count = 0;
    for (SetIter* iter = set_iter_begin(alive_regs_set); !set_iter_end(iter, alive_regs_set);
         iter = set_iter_next(iter, alive_regs_set)) {
        int* caller_ref = set_iter_item(iter, alive_regs_set);
        int caller_id = CALLER_SAVED_REG_IDS[*caller_ref];
        int callee_id = CALLEE_SAVED_REG_IDS[evacuation_count];
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[callee_id], QREG_NAMES[caller_id]);
        evacuation_count++;
    }
    if (x64gen->evacuation_count < evacuation_count) x64gen->evacuation_count = evacuation_count;
    delete_set(alive_regs_set);

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    char* dst_name = LREG_NAMES[dst_id];
    append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[AX_REG_ID], dst_name);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    append_code(codes, "\tpushq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[SP_REG_ID], QREG_NAMES[BP_REG_ID]);
    if (aligned_memory_size > 0) {
        append_code(codes, "\tsubq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* src = immc->inst->fst_src;
    int aligned_memory_size = ((src->imm_value + 15) / 16) * 16;

    if (aligned_memory_size) {
        append_code(codes, "\taddq\t$%d, %s\n", aligned_memory_size, QREG_NAMES[SP_REG_ID]);
    }
    append_code(codes, "\tpopq\t%s\n", QREG_NAMES[BP_REG_ID]);
    append_code(codes, "\tret\n");

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_prep_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* fst_src = immc->inst->fst_src;

    if (fst_src->imm_value > NUM_ARG_REGS && fst_src->imm_value % 2 == 1) {
        append_code(codes, "\tsubq\t$%d, %s\n", 8, QREG_NAMES[SP_REG_ID]);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_clean_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* fst_src = immc->inst->fst_src;

    if (fst_src->imm_value > NUM_ARG_REGS) {
        int mem_param_offset = ((fst_src->imm_value - NUM_ARG_REGS + 1) / 2) * 16;
        append_code(codes, "\taddq\t$%d, %s\n", mem_param_offset, QREG_NAMES[SP_REG_ID]);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
