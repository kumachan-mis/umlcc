#include "./ctrlinst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_jcmp_common_x64code(X64gen* x64gen, X64InstType type);

Vector* gen_jmp_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;

    X64Ope* dst = new_jlabel_x64ope(new_string(immc_dst->label_name));
    vector_push(codes, new_inst_x64(X64_INST_JMP, dst, NULL));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_jeq_x64code(X64gen* x64gen) {
    return gen_jcmp_common_x64code(x64gen, X64_INST_JE);
}

Vector* gen_jneq_x64code(X64gen* x64gen) {
    return gen_jcmp_common_x64code(x64gen, X64_INST_JNE);
}

Vector* gen_jcmp_common_x64code(X64gen* x64gen, X64InstType type) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            X64Suffix suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_CMPX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Suffix suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);
            int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];
            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg_id];
            append_mov_code(codes, fst_src_id, fst_src_suffix, fst_src_id, suffix);
            append_mov_code(codes, snd_src_id, snd_src_suffix, snd_src_id, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_CMPX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_dst->type);
            exit(1);
    }

    X64Ope* dst = new_jlabel_x64ope(new_string(immc_dst->label_name));
    vector_push(codes, new_inst_x64(type, dst, NULL));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_call_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    Set* alive_regs_set = create_alive_regs_set(x64gen->liveseqs);
    int evacuation_count = 0;
    // TODO: consider case that evacuation_count is greater than NUM_CALLEE_SAVED_REG
    for (SetIter* iter = set_iter_begin(alive_regs_set); !set_iter_end(iter, alive_regs_set);
         iter = set_iter_next(iter, alive_regs_set)) {
        int* caller_ref = set_iter_item(iter, alive_regs_set);
        int caller_id = CALLER_SAVED_REG_IDS[*caller_ref];
        int callee_id = CALLEE_SAVED_REG_IDS[evacuation_count];
        append_mov_code(codes, caller_id, X64_SUFFIX_QUAD, callee_id, X64_SUFFIX_QUAD);
        evacuation_count++;
    }

    int num_args = immc_snd_src->iliteral->signed_value;
    int src_id = -1;
    if (immc_fst_src->type == IMMC_OPERAND_PTR) {
        src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];
        Set* arg_regs_set = create_arg_regs_set(num_args);
        if (set_contains(arg_regs_set, &src_id)) {
            int evacuation_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
            append_mov_code(codes, src_id, X64_SUFFIX_QUAD, evacuation_id, X64_SUFFIX_QUAD);
            src_id = evacuation_id;
        }
        delete_set(arg_regs_set);
    }

    for (int i = 0; i < num_args && i < NUM_ARG_REGS; i++) {
        X64Ope* arg = new_reg_x64ope(X64_SUFFIX_QUAD, ARG_REG_IDS[i]);
        vector_push(codes, new_inst_x64(X64_INST_POPX, arg, NULL));
    }

    X64Ope* ret = new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID);
    X64Ope* zero = new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0);
    vector_push(codes, new_inst_x64(X64_INST_MOVX, zero, ret));

    switch (immc_fst_src->type) {
        case IMMC_OPERAND_PTR: {
            X64Ope* src = new_jptr_x64ope(src_id);
            vector_push(codes, new_inst_x64(X64_INST_CALL, src, NULL));
            break;
        }
        case IMMC_OPERAND_LABEL: {
            X64Ope* src = new_jlabel_x64ope(new_string(immc_fst_src->label_name));
            vector_push(codes, new_inst_x64(X64_INST_CALL, src, NULL));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_fst_src->type);
            exit(1);
    }

    evacuation_count = 0;
    for (SetIter* iter = set_iter_begin(alive_regs_set); !set_iter_end(iter, alive_regs_set);
         iter = set_iter_next(iter, alive_regs_set)) {
        int* caller_ref = set_iter_item(iter, alive_regs_set);
        int caller_id = CALLER_SAVED_REG_IDS[*caller_ref];
        int callee_id = CALLEE_SAVED_REG_IDS[evacuation_count];
        append_mov_code(codes, callee_id, X64_SUFFIX_QUAD, caller_id, X64_SUFFIX_QUAD);
        evacuation_count++;
    }
    if (x64gen->evacuation_count < evacuation_count) x64gen->evacuation_count = evacuation_count;
    delete_set(alive_regs_set);

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
    append_mov_code(codes, AX_REG_ID, dst_suffix, dst_id, dst_suffix);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;
    int aligned_memory_size = ((immc_src->iliteral->signed_value + 15) / 16) * 16;

    X64Ope* bp = NULL;
    X64Ope* sp = NULL;
    bp = new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID);
    vector_push(codes, new_inst_x64(X64_INST_PUSHX, bp, NULL));

    bp = new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID);
    sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
    vector_push(codes, new_inst_x64(X64_INST_MOVX, sp, bp));

    if (aligned_memory_size > 0) {
        X64Ope* imm = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, aligned_memory_size);
        sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_SUBX, imm, sp));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;
    int aligned_memory_size = ((immc_src->iliteral->signed_value + 15) / 16) * 16;

    if (aligned_memory_size > 0) {
        X64Ope* imm = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, aligned_memory_size);
        X64Ope* sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_ADDX, imm, sp));
    }
    X64Ope* bp = new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID);
    vector_push(codes, new_inst_x64(X64_INST_POPX, bp, NULL));
    vector_push(codes, new_inst_x64(X64_INST_RET, NULL, NULL));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_prep_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;

    int num_args = immc_src->iliteral->signed_value;
    if (num_args > NUM_ARG_REGS && num_args % 2 == 1) {
        X64Ope* imm = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8);
        X64Ope* sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_SUBX, imm, sp));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_clean_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;

    int num_args = immc_src->iliteral->signed_value;
    if (num_args > NUM_ARG_REGS) {
        int mem_param_offset = ((num_args - NUM_ARG_REGS + 1) / 2) * 16;
        X64Ope* imm = new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, mem_param_offset);
        X64Ope* sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_ADDX, imm, sp));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
