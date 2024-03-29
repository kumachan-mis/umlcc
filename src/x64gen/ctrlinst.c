#include "./ctrlinst.h"
#include "../common/util.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_jcmp_common_x64code(X64gen* x64gen, X64InstType type);

Vector* gen_jmp_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;

    X64Ope* dst = new_jlabel_x64ope(new_string(immc_dst->label_name));
    vector_push(codes, new_inst_x64(X64_INST_JMP, NULL, dst));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_jeq_x64code(X64gen* x64gen) {
    return gen_jcmp_common_x64code(x64gen, X64_INST_JE);
}

Vector* gen_jneq_x64code(X64gen* x64gen) {
    return gen_jcmp_common_x64code(x64gen, X64_INST_JNE);
}

Vector* gen_jgt_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    ImmcOpe* immc_fst_src = immc->inst->fst_src;

    if (immc_fst_src->reg->is_unsigned) {
        return gen_jcmp_common_x64code(x64gen, X64_INST_JA);
    }
    return gen_jcmp_common_x64code(x64gen, X64_INST_JG);
}

Vector* gen_jgeq_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    ImmcOpe* immc_fst_src = immc->inst->fst_src;

    if (immc_fst_src->reg->is_unsigned) {
        return gen_jcmp_common_x64code(x64gen, X64_INST_JAE);
    }
    return gen_jcmp_common_x64code(x64gen, X64_INST_JGE);
}

Vector* gen_jlt_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    ImmcOpe* immc_fst_src = immc->inst->fst_src;

    if (immc_fst_src->reg->is_unsigned) {
        return gen_jcmp_common_x64code(x64gen, X64_INST_JB);
    }
    return gen_jcmp_common_x64code(x64gen, X64_INST_JL);
}

Vector* gen_jleq_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    ImmcOpe* immc_fst_src = immc->inst->fst_src;

    if (immc_fst_src->reg->is_unsigned) {
        return gen_jcmp_common_x64code(x64gen, X64_INST_JBE);
    }
    return gen_jcmp_common_x64code(x64gen, X64_INST_JLE);
}

Vector* gen_jcmp_common_x64code(X64gen* x64gen, X64InstType type) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg->reg_id];

    X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_fst_src->suffix));
    X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_snd_src->suffix));
    X64Suffix suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);

    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            X64Ope* fst_src_mov_dst = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* fst_src_mov_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            int fst_src_is_unsigned = immc_fst_src->reg->is_unsigned;
            append_mov_code(codes, fst_src_mov_src, fst_src_mov_dst, fst_src_is_unsigned);

            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_CMPX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Ope* fst_src_mov_dst = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* fst_src_mov_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            int fst_src_is_unsigned = immc_fst_src->reg->is_unsigned;
            append_mov_code(codes, fst_src_mov_src, fst_src_mov_dst, fst_src_is_unsigned);

            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg->reg_id];
            X64Ope* snd_src_mov_dst = new_reg_x64ope(suffix, snd_src_id);
            X64Ope* snd_src_mov_src = new_reg_x64ope(snd_src_suffix, snd_src_id);
            int snd_src_is_unsigned = immc_snd_src->reg->is_unsigned;
            append_mov_code(codes, snd_src_mov_src, snd_src_mov_dst, snd_src_is_unsigned);

            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_CMPX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_jcmp_common_x64code)\n");
            exit(1);
    }

    X64Ope* dst = new_jlabel_x64ope(new_string(immc_dst->label_name));
    vector_push(codes, new_inst_x64(type, NULL, dst));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_call_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    Set* alive_reg_induces_set = create_alive_reg_induces_set(x64gen->liveseqs);
    int evacuation_count = 0;
    // TODO: consider case that evacuation_count is greater than NUM_CALLEE_SAVED_REG
    for (SetIter* iter = set_iter_begin(alive_reg_induces_set); !set_iter_end(iter, alive_reg_induces_set);
         iter = set_iter_next(iter, alive_reg_induces_set)) {
        int* caller_ref = set_iter_item(iter, alive_reg_induces_set);
        if (*caller_ref == immc->inst->dst->reg->reg_id) {
            continue;
        }
        int callee_id = CALLEE_SAVED_REG_IDS[evacuation_count];
        X64Ope* callee = new_reg_x64ope(X64_SUFFIX_QUAD, callee_id);
        int caller_id = CALLER_SAVED_REG_IDS[*caller_ref];
        X64Ope* caller = new_reg_x64ope(X64_SUFFIX_QUAD, caller_id);
        append_mov_code(codes, caller, callee, 0);
        evacuation_count++;
    }

    int num_args = immc_snd_src->iliteral->signed_value;
    int src_id = -1;
    if (immc_fst_src->type == IMMC_OPERAND_PTR) {
        src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg->reg_id];
        Set* arg_regs_set = create_arg_regs_set(num_args);
        if (set_contains(arg_regs_set, &src_id)) {
            int evacuation_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
            X64Ope* evacuation_dst = new_reg_x64ope(X64_SUFFIX_QUAD, evacuation_id);
            X64Ope* evacuation_src = new_reg_x64ope(X64_SUFFIX_QUAD, src_id);
            append_mov_code(codes, evacuation_src, evacuation_dst, 0);
            src_id = evacuation_id;
        }
        delete_set(arg_regs_set);
    }

    for (int i = 0; i < num_args && i < NUM_ARG_REGS; i++) {
        X64Ope* arg = new_reg_x64ope(X64_SUFFIX_QUAD, ARG_REG_IDS[i]);
        vector_push(codes, new_inst_x64(X64_INST_POPX, NULL, arg));
    }

    X64Ope* ret = new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID);
    X64Ope* zero = new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0);
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
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_call_x64code)\n");
            exit(1);
    }

    evacuation_count = 0;
    for (SetIter* iter = set_iter_begin(alive_reg_induces_set); !set_iter_end(iter, alive_reg_induces_set);
         iter = set_iter_next(iter, alive_reg_induces_set)) {
        int* caller_ref = set_iter_item(iter, alive_reg_induces_set);
        if (*caller_ref == immc->inst->dst->reg->reg_id) {
            continue;
        }
        int caller_id = CALLER_SAVED_REG_IDS[*caller_ref];
        X64Ope* caller = new_reg_x64ope(X64_SUFFIX_QUAD, caller_id);
        int callee_id = CALLEE_SAVED_REG_IDS[evacuation_count];
        X64Ope* callee = new_reg_x64ope(X64_SUFFIX_QUAD, callee_id);
        append_mov_code(codes, callee, caller, 0);
        evacuation_count++;
    }
    if (x64gen->evacuation_count < evacuation_count) {
        x64gen->evacuation_count = evacuation_count;
    }
    delete_set(alive_reg_induces_set);

    if (immc_dst != NULL) {
        X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immc_dst->suffix));
        int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id];

        X64Ope* dst_mov_dst = new_reg_x64ope(dst_suffix, dst_id);
        X64Ope* dst_mov_src = new_reg_x64ope(dst_suffix, AX_REG_ID);
        int dst_is_unsigned = immc_dst->reg->is_unsigned;
        append_mov_code(codes, dst_mov_src, dst_mov_dst, dst_is_unsigned);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_enter_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;
    int aligned_memory_size = min_multiple_of(immc_src->iliteral->signed_value, 16);

    X64Ope* bp = NULL;
    X64Ope* sp = NULL;
    bp = new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID);
    vector_push(codes, new_inst_x64(X64_INST_PUSHX, bp, NULL));

    bp = new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID);
    sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
    vector_push(codes, new_inst_x64(X64_INST_MOVX, sp, bp));

    if (aligned_memory_size > 0) {
        X64Ope* imm = new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, aligned_memory_size);
        sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_SUBX, imm, sp));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_leave_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;
    int aligned_memory_size = min_multiple_of(immc_src->iliteral->signed_value, 16);

    if (aligned_memory_size > 0) {
        X64Ope* imm = new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, aligned_memory_size);
        X64Ope* sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_ADDX, imm, sp));
    }
    X64Ope* bp = new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID);
    vector_push(codes, new_inst_x64(X64_INST_POPX, NULL, bp));
    vector_push(codes, new_inst_x64(X64_INST_RET, NULL, NULL));

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_prep_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;

    int num_args = immc_src->iliteral->signed_value;
    if (num_args > NUM_ARG_REGS && num_args % 2 == 1) {
        X64Ope* imm = new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8);
        X64Ope* sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_SUBX, imm, sp));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_clean_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_src = immc->inst->fst_src;

    int num_args = immc_src->iliteral->signed_value;
    if (num_args > NUM_ARG_REGS) {
        int mem_param_offset = ((num_args - NUM_ARG_REGS + 1) / 2) * 16;
        X64Ope* imm = new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, mem_param_offset);
        X64Ope* sp = new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_ADDX, imm, sp));
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
