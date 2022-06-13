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

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            X64Suffix fst_src_suffix = x64suffix_get(immcsuffix_tosize(fst_src->suffix));
            int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
            X64Ope* fst_src = new_reg_x64ope(fst_src_suffix, fst_src_id);
            X64Ope* snd_src = new_imm_x64ope(snd_src->imm_value);
            append_code(codes, new_inst_x64(type, snd_src, fst_src));
            X64Suffix dst_suffix = x64suffix_get(immcsuffix_tosize(dst->suffix));
            int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
            append_mov_code(codes, fst_src_id, fst_src_suffix, dst_id, dst_suffix);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
