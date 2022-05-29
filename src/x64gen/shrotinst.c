#include "./shrotinst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_shrot_common_x64code(X64gen* x64gen, char* inst);

Vector* gen_sal_x64code(X64gen* x64gen) {
    return gen_shrot_common_x64code(x64gen, "sal");
}

Vector* gen_sar_x64code(X64gen* x64gen) {
    return gen_shrot_common_x64code(x64gen, "sar");
}

Vector* gen_shrot_common_x64code(X64gen* x64gen, char* inst) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            int src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
            ImmcSuffix immc_suffix = fst_src->suffix;
            char* src_name = reg_name(src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\t%s%c\t$%d, %s\n", inst, suffix, snd_src->imm_value, src_name);

            int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
            append_mov_code(codes, src_id, immc_suffix, dst_id, dst->suffix);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", snd_src->type);
            exit(1);
    }

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
