#include "./logicinst.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_not_x64code(X64gen* x64gen) {
    Vector* codes = new_vector(&t_x64);
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    x64gen->index++;

    ImmcOpe* immc_dst = immc->inst->dst;
    ImmcOpe* immc_src = immc->inst->fst_src;

    int dst_id = CALLER_SAVED_REG_IDS[immc_dst->reg->reg_id];
    int src_id = -1;

    X64Suffix src_suffix = x64suffix_get(immcsuffix_tonbytes(immc_src->suffix));
    X64Suffix dst_suffix = x64suffix_get(immcsuffix_tonbytes(immc_dst->suffix));

    int is_unsigned = 0;

    switch (immc_src->type) {
        case IMMC_OPERAND_INT: {
            src_id = dst_id;
            is_unsigned = iliteral_isunsigned(immc_src->iliteral);

            X64Ope* dst = new_reg_x64ope(src_suffix, src_id);
            X64Ope* src = new_int_x64ope(src_suffix, iliteral_copy(immc_src->iliteral));
            vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
            break;
        }
        case IMMC_OPERAND_REG:
            src_id = CALLER_SAVED_REG_IDS[immc_src->reg->reg_id];
            is_unsigned = immc_src->reg->is_unsigned;
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in gen_not_x64code src)\n");
            exit(1);
    }

    X64Ope* operand = new_reg_x64ope(src_suffix, src_id);
    vector_push(codes, new_inst_x64(X64_INST_NOTX, NULL, operand));

    X64Ope* dst = new_reg_x64ope(dst_suffix, dst_id);
    X64Ope* src = new_reg_x64ope(src_suffix, src_id);
    append_mov_code(codes, src, dst, is_unsigned);

    liveseqs_next(x64gen->liveseqs);
    return codes;
}
