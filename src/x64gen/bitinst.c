#include "./bitinst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_setcmp_common_x64code(X64gen* x64gen, X64InstType type);

Vector* gen_seteq_x64code(X64gen* x64gen) {
    return gen_setcmp_common_x64code(x64gen, X64_INST_SETE);
}

Vector* gen_setneq_x64code(X64gen* x64gen) {
    return gen_setcmp_common_x64code(x64gen, X64_INST_SETNE);
}

Vector* gen_setcmp_common_x64code(X64gen* x64gen, X64InstType type) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];

    X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
    X64Suffix snd_src_suffix = x64suffix_get(immcsuffix_tosize(immc_snd_src->suffix));
    X64Suffix suffix = x64suffix_greater(fst_src_suffix, snd_src_suffix);

    switch (immc_snd_src->type) {
        case IMMC_OPERAND_INT: {
            append_mov_code(codes, fst_src_id, fst_src_suffix, fst_src_id, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_int_x64ope(suffix, iliteral_copy(immc_snd_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_CMPX, snd_src, fst_src));
            break;
        }
        case IMMC_OPERAND_REG: {
            int snd_src_id = CALLER_SAVED_REG_IDS[immc_snd_src->reg_id];
            append_mov_code(codes, fst_src_id, fst_src_suffix, fst_src_id, suffix);
            append_mov_code(codes, snd_src_id, snd_src_suffix, snd_src_id, suffix);
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_reg_x64ope(suffix, snd_src_id);
            vector_push(codes, new_inst_x64(X64_INST_CMPX, snd_src, fst_src));
            break;
        }
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: unreachable statement"
                            " (in gen_setcmp_common_x64code)\n");
            exit(1);
    }

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
    X64Ope* dst = new_reg_x64ope(X64_SUFFIX_BYTE, dst_id);
    vector_push(codes, new_inst_x64(type, NULL, dst));

    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
    append_mov_code(codes, dst_id, X64_SUFFIX_BYTE, dst_id, dst_suffix);
    liveseqs_next(x64gen->liveseqs);
    return codes;
}
