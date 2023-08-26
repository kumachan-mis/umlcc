#include "./util.h"
#include "../liveseq/liveseq.h"
#include "../x64/x64.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_mov_code(Vector* codes, X64Ope* src, X64Ope* dst, int is_unsigned) {
    void append_mov_code_simple(Vector * codes, X64Ope * src, X64Ope * dst);
    void append_mov_code_unsigned(Vector * codes, X64Ope * src, X64Ope * dst);
    void append_mov_code_signed(Vector * codes, X64Ope * src, X64Ope * dst);

    if (src->suffix >= dst->suffix) {
        append_mov_code_simple(codes, src, dst);
        return;
    }

    if (is_unsigned) {
        append_mov_code_unsigned(codes, src, dst);
    } else {
        append_mov_code_signed(codes, src, dst);
    }
}

void append_mov_code_simple(Vector* codes, X64Ope* src, X64Ope* dst) {
    if (src->reg_id == dst->reg_id) {
        delete_x64ope(src);
        delete_x64ope(dst);
        return;
    }
    src->suffix = dst->suffix;
    vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
}

void append_mov_code_unsigned(Vector* codes, X64Ope* src, X64Ope* dst) {
    if (src->suffix == X64_SUFFIX_BYTE || src->suffix == X64_SUFFIX_WORD) {
        vector_push(codes, new_inst_x64(X64_INST_MOVZXX, src, dst));
        return;
    }
    if (src->reg_id == dst->reg_id) {
        delete_x64ope(src);
        delete_x64ope(dst);
        return;
    }
    vector_push(codes, new_inst_x64(X64_INST_MOVX, src, dst));
}

void append_mov_code_signed(Vector* codes, X64Ope* src, X64Ope* dst) {
    if (src->suffix == X64_SUFFIX_BYTE || src->suffix == X64_SUFFIX_WORD) {
        vector_push(codes, new_inst_x64(X64_INST_MOVSXX, src, dst));
        return;
    }

    X64Suffix src_suffix = src->suffix;
    X64Suffix dst_suffix = dst->suffix;

    if (src->reg_id != AX_REG_ID) {
        X64Ope* ax_dst = new_reg_x64ope(src->suffix, AX_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, src, ax_dst));
    } else {
        delete_x64ope(src);
    }

    X64Ope* suffix_src = new_suffix_x64ope(src_suffix);
    X64Ope* suffix_dst = new_suffix_x64ope(dst_suffix);
    vector_push(codes, new_inst_x64(X64_INST_CXTX, suffix_src, suffix_dst));

    if (dst->reg_id != AX_REG_ID) {
        X64Ope* ax_src = new_reg_x64ope(dst->suffix, AX_REG_ID);
        vector_push(codes, new_inst_x64(X64_INST_MOVX, ax_src, dst));
    } else {
        delete_x64ope(dst);
    }
}

void liveseqs_next(Vector* liveseqs) {
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_id = 0; reg_id < liveseqs_len; reg_id++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_id);
        liveseq_goto_next(liveseq);
    }
}

Set* create_alive_reg_induces_set(Vector* liveseqs) {
    Set* alive_regs_set = new_set(&t_hashable_integer);
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_index = 0; reg_index < liveseqs_len; reg_index++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_index);
        if (liveseq_isalive(liveseq)) {
            set_add(alive_regs_set, new_integer(reg_index));
        }
    }
    return alive_regs_set;
}

Set* create_alive_regs_set(Vector* liveseqs) {
    Set* alive_regs_set = new_set(&t_hashable_integer);
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_index = 0; reg_index < liveseqs_len; reg_index++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_index);
        int reg_id = CALLER_SAVED_REG_IDS[reg_index];
        if (liveseq_isalive(liveseq)) {
            set_add(alive_regs_set, new_integer(reg_id));
        }
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
