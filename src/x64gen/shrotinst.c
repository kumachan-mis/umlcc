#include "./shrotinst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_shrot_common_x64code(X64gen* x64gen, X64InstType type);

Vector* gen_sal_x64code(X64gen* x64gen) {
    return gen_shrot_common_x64code(x64gen, X64_INST_SALX);
}

Vector* gen_sar_x64code(X64gen* x64gen) {
    return gen_shrot_common_x64code(x64gen, X64_INST_SARX);
}

Vector* gen_shrot_common_x64code(X64gen* x64gen, X64InstType type) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_fst_src = immc->inst->fst_src;
    ImmcOpe* immc_snd_src = immc->inst->snd_src;

    switch (immc_snd_src->type) {
        case IMMC_OPERAND_IMM: {
            X64Suffix suffix = x64suffix_get(immcsuffix_tosize(immc_fst_src->suffix));
            int fst_src_id = CALLER_SAVED_REG_IDS[immc_fst_src->reg_id];
            X64Ope* fst_src = new_reg_x64ope(suffix, fst_src_id);
            X64Ope* snd_src = new_signed_x64ope(suffix, immc_snd_src->imm_value);
            vector_push(codes, new_inst_x64(type, snd_src, fst_src));
            X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(immc_dst->suffix));
            int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg_id];
            append_mov_code(codes, fst_src_id, suffix, dst_id, dst_suffix);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", immc_snd_src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
