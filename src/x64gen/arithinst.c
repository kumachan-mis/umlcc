#include "./arithinst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_divisional_common_x64code(X64gen* x64gen, int result_reg_id);

Vector* gen_add_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];

    X64Suffix suffix = X64_SUFFIX_NONE;
    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_ADDX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg_id];
            X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tosize(immc_snd_src->suffix));
            suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);
            append_mov_code(codes, fst_src_id, fst_src_suffix, fst_src_id, suffix);
            append_mov_code(codes, snd_src_id, snd_src_suffix, snd_src_id, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_ADDX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_snd_src->type);
            exit(1);
    }

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    append_mov_code(codes, fst_src_id, suffix, dst_id, dst_suffix);
    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_sub_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];

    X64Suffix suffix = X64_SUFFIX_NONE;
    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_SUBX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg_id];
            X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tosize(immc_snd_src->suffix));
            suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);
            append_mov_code(codes, fst_src_id, fst_src_suffix, fst_src_id, suffix);
            append_mov_code(codes, snd_src_id, snd_src_suffix, snd_src_id, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_SUBX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_snd_src->type);
            exit(1);
    }

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    append_mov_code(codes, fst_src_id, suffix, dst_id, dst_suffix);
    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_mul_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];

    X64Suffix suffix = X64_SUFFIX_NONE;
    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            append_mov_code(codes, fst_src_id, suffix, AX_REG_ID, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_IMULX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg_id];
            X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tosize(immc_snd_src->suffix));
            suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);
            append_mov_code(codes, fst_src_id, fst_src_suffix, AX_REG_ID, suffix);
            append_mov_code(codes, snd_src_id, snd_src_suffix, snd_src_id, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_IMULX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_snd_src->type);
            exit(1);
    }

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    append_mov_code(codes, AX_REG_ID, suffix, dst_id, dst_suffix);
    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_div_x64code(X64gen* x64gen) {
    return gen_divisional_common_x64code(x64gen, AX_REG_ID);
}

Vector* gen_mod_x64code(X64gen* x64gen) {
    return gen_divisional_common_x64code(x64gen, DX_REG_ID);
}

Vector* gen_divisional_common_x64code(X64gen* x64gen, int result_reg_id) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];
    int snd_src_id = -1;

    X64Suffix suffix = X64_SUFFIX_NONE;
    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            snd_src_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];
            suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            append_mov_code(codes, fst_src_id, suffix, AX_REG_ID, suffix);
            X64Ope* dst = new_reg_x64ope(suffix, snd_src_id);
            X64Ope* src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_REG: {
            snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg_id];
            X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tosize(immc_snd_src->suffix));
            suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);
            append_mov_code(codes, fst_src_id, fst_src_suffix, AX_REG_ID, suffix);
            if (snd_src_id != DX_REG_ID) {
                append_mov_code(codes, snd_src_id, snd_src_suffix, snd_src_id, suffix);
            } else {
                append_mov_code(codes, snd_src_id, snd_src_suffix, fst_src_id, suffix);
                snd_src_id = fst_src_id;
            }
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_snd_src->type);
            exit(1);
    }

    vector_push(codes, new_inst_x64(X64_INST_CXTD, new_suffix_x64ope(suffix), NULL));

    X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
    vector_push(codes, new_inst_x64(X64_INST_IDIVX, snd_src, NULL));

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    append_mov_code(codes, result_reg_id, suffix, dst_id, dst_suffix);
    liveseqs_next(x64gen->liveseqs);
    return codes;
}
