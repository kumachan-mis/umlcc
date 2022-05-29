#include "./bitinst.h"
#include "../immc/immc.h"
#include "./register.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_setcmp_common_x64code(X64gen* x64gen, char* inst);

Vector* gen_seteq_x64code(X64gen* x64gen) {
    return gen_setcmp_common_x64code(x64gen, "sete");
}

Vector* gen_setneq_x64code(X64gen* x64gen) {
    return gen_setcmp_common_x64code(x64gen, "setne");
}

Vector* gen_setcmp_common_x64code(X64gen* x64gen, char* inst) {
    Vector* codes = new_vector(&t_string);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* dst = immc->inst->dst;
    ImmcOpe* fst_src = immc->inst->fst_src;
    ImmcOpe* snd_src = immc->inst->snd_src;

    switch (snd_src->type) {
        case IMMC_OPERAND_IMM: {
            int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
            ImmcSuffix immc_suffix = fst_src->suffix;
            char* fst_src_name = reg_name(fst_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\tcmp%c\t$%d, %s\n", suffix, snd_src->imm_value, fst_src_name);
            break;
        }
        case IMMC_OPERAND_REG: {
            int fst_src_id = CALLER_SAVED_REG_IDS[fst_src->reg_id];
            int snd_src_id = CALLER_SAVED_REG_IDS[snd_src->reg_id];
            ImmcSuffix immc_suffix = immcsuffix_max(fst_src->suffix, snd_src->suffix);
            char* fst_src_name = reg_name(fst_src_id, immc_suffix);
            char* snd_src_name = reg_name(snd_src_id, immc_suffix);
            char suffix = immcsuffix_tochar(immc_suffix);
            append_code(codes, "\tcmp%c\t%s, %s\n", suffix, snd_src_name, fst_src_name);
            break;
        }
        default:
            fprintf(stderr, "Error: unexpected operand %d\n", dst->type);
            exit(1);
    }

    int dst_id = CALLER_SAVED_REG_IDS[dst->reg_id];
    append_code(codes, "\t%s\t%s\n", inst, BREG_NAMES[dst_id]);
    append_mov_code(codes, dst_id, IMMC_SUFFIX_BYTE, dst_id, dst->suffix);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
