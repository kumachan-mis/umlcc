#include "./util.h"
#include "../liveseq/liveseq.h"
#include "../x64/x64.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_mov_code(Vector* codes, int src_reg_id, X64Suffix src_suffix, int dst_reg_id,
                     X64Suffix dst_suffix) {
    if (src_suffix >= dst_suffix) {
        if (src_reg_id == dst_reg_id) return;
        X64Ope* src = new_reg_x64ope(src_suffix, src_reg_id);
        X64Ope* dst = new_reg_x64ope(src_suffix, dst_reg_id);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
    } else {
        X64Ope* src = new_reg_x64ope(src_suffix, src_reg_id);
        X64Ope* dst = new_reg_x64ope(dst_suffix, dst_reg_id);
        vector_push(codes, new_inst_x64(X64_INST_MOVSXX, src, dst));
    }
}

void liveseqs_next(Vector* liveseqs) {
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_id = 0; reg_id < liveseqs_len; reg_id++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_id);
        liveseq_goto_next(liveseq);
    }
}

Set* create_alive_regs_set(Vector* liveseqs) {
    Set* alive_regs_set = new_set(&t_hashable_integer);
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_id = 0; reg_id < liveseqs_len; reg_id++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_id);
        if (liveseq_is_alive(liveseq)) set_add(alive_regs_set, new_integer(reg_id));
    }
    return alive_regs_set;
}

Set* create_arg_regs_set(int num_args) {
    Set* arg_regs_set = new_set(&t_hashable_integer);
    for (int i = 0; i < num_args && i < NUM_ARG_REGS; i++) {
        int arg_id = ARG_REG_IDS[i];
        set_add(arg_regs_set, new_integer(arg_id));
    }
    return arg_regs_set;
}
