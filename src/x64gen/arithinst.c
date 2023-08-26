#include "./arithinst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_additive_subtractive_common_x64code(X64gen* x64gen, X64InstType type);
Vector* gen_divisional_common_x64code(X64gen* x64gen, X64RegisterId result_reg_id);

Vector* gen_add_x64code(X64gen* x64gen) {
    return gen_additive_subtractive_common_x64code(x64gen, X64_INST_ADDX);
}

Vector* gen_sub_x64code(X64gen* x64gen) {
    return gen_additive_subtractive_common_x64code(x64gen, X64_INST_SUBX);
}

Vector* gen_additive_subtractive_common_x64code(X64gen* x64gen, X64InstType type) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id];
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
            vector_push(codes, new_inst_x64(type, snd_src, fst_src));
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
            vector_push(codes, new_inst_x64(type, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_additive_common_x64code snd_src)\n");
            exit(1);
    }

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immc_dst->suffix));

    X64Ope* dst_mov_dst = new_reg_x64ope(dst_suffix, dst_id);
    X64Ope* dst_mov_src = new_reg_x64ope(suffix, fst_src_id);
    int dst_is_unsigned = immc_dst->reg->is_unsigned;
    append_mov_code(codes, dst_mov_src, dst_mov_dst, dst_is_unsigned);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_mul_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg->reg_id];

    X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_fst_src->suffix));
    X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_snd_src->suffix));
    X64Suffix suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);

    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            X64Ope* fst_src_mov_dst = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* fst_src_mov_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            int fst_src_is_unsigned = immc_fst_src->reg->is_unsigned;
            append_mov_code(codes, fst_src_mov_src, fst_src_mov_dst, fst_src_is_unsigned);

            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            X64Ope* fst_src = new_reg_x64ope(suffix, AX_REG_ID);
            vector_push(codes, new_inst_x64(X64_INST_IMULX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            X64Ope* fst_src_mov_dst = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* fst_src_mov_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            int fst_src_is_unsigned = immc_fst_src->reg->is_unsigned;
            append_mov_code(codes, fst_src_mov_src, fst_src_mov_dst, fst_src_is_unsigned);

            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg->reg_id];
            X64Ope* snd_src_mov_dst = new_reg_x64ope(suffix, snd_src_id);
            X64Ope* snd_src_mov_src = new_reg_x64ope(snd_src_suffix, snd_src_id);
            int snd_src_is_unsigned = immc_snd_src->reg->is_unsigned;
            append_mov_code(codes, snd_src_mov_src, snd_src_mov_dst, snd_src_is_unsigned);

            X64Ope* fst_src = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_IMULX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_mul_x64code)\n");
            exit(1);
    }

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immc_dst->suffix));

    X64Ope* dst_mov_dst = new_reg_x64ope(dst_suffix, dst_id);
    X64Ope* dst_mov_src = new_reg_x64ope(suffix, AX_REG_ID);
    int dst_is_unsigned = immc_dst->reg->is_unsigned;
    append_mov_code(codes, dst_mov_src, dst_mov_dst, dst_is_unsigned);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}

Vector* gen_div_x64code(X64gen* x64gen) {
    return gen_divisional_common_x64code(x64gen, AX_REG_ID);
}

Vector* gen_mod_x64code(X64gen* x64gen) {
    return gen_divisional_common_x64code(x64gen, DX_REG_ID);
}

Vector* gen_divisional_common_x64code(X64gen* x64gen, X64RegisterId result_reg_id) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id];
    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg->reg_id];
    int snd_src_id = -1;

    X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_fst_src->suffix));
    X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_snd_src->suffix));
    X64Suffix suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);

    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            snd_src_id = CALLER_SAVED_REG_IDS[NUM_CALLER_SAVED_REGS - 2];

            X64Ope* fst_src_mov_dst = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* fst_src_mov_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            int fst_src_is_unsigned = immc_fst_src->reg->is_unsigned;
            append_mov_code(codes, fst_src_mov_src, fst_src_mov_dst, fst_src_is_unsigned);

            X64Ope* dst = new_reg_x64ope(suffix, snd_src_id);
            X64Ope* src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_REG: {
            snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg->reg_id];

            X64Ope* fst_src_mov_dst = new_reg_x64ope(suffix, AX_REG_ID);
            X64Ope* fst_src_mov_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            int fst_src_is_unsigned = immc_fst_src->reg->is_unsigned;
            append_mov_code(codes, fst_src_mov_src, fst_src_mov_dst, fst_src_is_unsigned);

            if (snd_src_id != DX_REG_ID) {
                X64Ope* snd_src_mov_dst = new_reg_x64ope(suffix, snd_src_id);
                X64Ope* snd_src_mov_src = new_reg_x64ope(snd_src_suffix, snd_src_id);
                int snd_src_is_unsigned = immc_snd_src->reg->is_unsigned;
                append_mov_code(codes, snd_src_mov_src, snd_src_mov_dst, snd_src_is_unsigned);
            } else {
                X64Ope* snd_src_mov_dst = new_reg_x64ope(suffix, fst_src_id);
                X64Ope* snd_src_mov_src = new_reg_x64ope(snd_src_suffix, snd_src_id);
                int snd_src_is_unsigned = immc_snd_src->reg->is_unsigned;
                append_mov_code(codes, snd_src_mov_src, snd_src_mov_dst, snd_src_is_unsigned);
                snd_src_id = fst_src_id;
            }
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_divisional_common_x64code snd_src)\n");
            exit(1);
    }

    int dx_needs_evacuation = 0;
    if (fst_src_id != DX_REG_ID && snd_src_id != DX_REG_ID && dst_id != DX_REG_ID) {
        Set* alive_regs_set = create_alive_regs_set(x64gen->liveseqs);
        int* dx_reg_id_ref = new_integer(DX_REG_ID);
        dx_needs_evacuation = set_contains(alive_regs_set, dx_reg_id_ref);
        free(dx_reg_id_ref);
        delete_set(alive_regs_set);
    }

    if (dx_needs_evacuation) {
        int evacuation_id = CALLEE_SAVED_REG_IDS[0];
        X64Ope* evacuation_dst = new_reg_x64ope(X64_SUFFIX_QUAD, evacuation_id);
        X64Ope* evacuation_src = new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID);
        append_mov_code(codes, evacuation_src, evacuation_dst, 0);
    }

    X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
    if (immc_dst->reg->is_unsigned) {
        X64Ope* dx_dst = new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID);
        X64Ope* zero_src = new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, zero_src, dx_dst));
    } else if (suffix != X64_SUFFIX_QUAD) {
        vector_push(codes, new_inst_x64(X64_INST_CXTD, new_suffix_x64ope(suffix), NULL));
    } else {
        vector_push(codes, new_inst_x64(X64_INST_CQTO, NULL, NULL));
    }

    if (immc_dst->reg->is_unsigned) {
        vector_push(codes, new_inst_x64(X64_INST_DIVX, snd_src, NULL));
    } else {
        vector_push(codes, new_inst_x64(X64_INST_IDIVX, snd_src, NULL));
    }

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immc_dst->suffix));

    X64Ope* dst_mov_dst = new_reg_x64ope(dst_suffix, dst_id);
    X64Ope* dst_mov_src = new_reg_x64ope(suffix, result_reg_id);
    int dst_is_unsigned = immc_dst->reg->is_unsigned;
    append_mov_code(codes, dst_mov_src, dst_mov_dst, dst_is_unsigned);

    if (dx_needs_evacuation) {
        int evacuation_id = CALLEE_SAVED_REG_IDS[0];
        X64Ope* evacuation_dst = new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID);
        X64Ope* evacuation_src = new_reg_x64ope(X64_SUFFIX_QUAD, evacuation_id);
        append_mov_code(codes, evacuation_src, evacuation_dst, 0);
        if (x64gen->evacuation_count < 1) {
            x64gen->evacuation_count = 1;
        }
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
