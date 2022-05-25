#include "./util.h"
#include "../liveseq/liveseq.h"
#include "./register.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_code(Vector* codes, char* format, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, format);

    char* code = malloc((100 + 1) * sizeof(char));
    vsprintf(code, format, arg_ptr);
    code = realloc(code, (strlen(code) + 1) * sizeof(char));
    vector_push(codes, code);

    va_end(arg_ptr);
}

void append_mov_code(Vector* codes, ImmcOpe* src, ImmcOpe* dst) {
    if (src->reg_id == dst->reg_id && src->suffix >= dst->suffix) return;

    if (src->suffix == SUFFIX_BYTE && dst->suffix == SUFFIX_BYTE) {
        append_code(codes, "\tmovb\t%s, %s\n", BREG_NAMES[src->reg_id], BREG_NAMES[dst->reg_id]);
    } else if (src->suffix == SUFFIX_BYTE && dst->suffix == SUFFIX_WORD) {
        append_code(codes, "\tmovsbw\t%s, %s\n", BREG_NAMES[src->reg_id], WREG_NAMES[dst->reg_id]);
    } else if (src->suffix == SUFFIX_BYTE && dst->suffix == SUFFIX_LONG) {
        append_code(codes, "\tmovsbl\t%s, %s\n", BREG_NAMES[src->reg_id], LREG_NAMES[dst->reg_id]);
    } else if (src->suffix == SUFFIX_BYTE && dst->suffix == SUFFIX_QUAD) {
        append_code(codes, "\tmovsbq\t%s, %s\n", BREG_NAMES[src->reg_id], QREG_NAMES[dst->reg_id]);
    }

    else if (src->suffix == SUFFIX_WORD && SUFFIX_BYTE <= dst->suffix &&
             dst->suffix <= SUFFIX_WORD) {
        append_code(codes, "\tmovw\t%s, %s\n", WREG_NAMES[src->reg_id], WREG_NAMES[dst->reg_id]);
    } else if (src->suffix == SUFFIX_WORD && dst->suffix == SUFFIX_LONG) {
        append_code(codes, "\tmovswl\t%s, %s\n", WREG_NAMES[src->reg_id], LREG_NAMES[dst->reg_id]);
    } else if (src->suffix == SUFFIX_WORD && dst->suffix == SUFFIX_QUAD) {
        append_code(codes, "\tmovswq\t%s, %s\n", WREG_NAMES[src->reg_id], QREG_NAMES[dst->reg_id]);
    }

    else if (src->suffix == SUFFIX_LONG && SUFFIX_BYTE <= dst->suffix &&
             dst->suffix <= SUFFIX_LONG) {
        append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[src->reg_id], LREG_NAMES[dst->reg_id]);
    } else if (src->suffix == SUFFIX_LONG && dst->suffix == SUFFIX_QUAD) {
        append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[src->reg_id], LREG_NAMES[AX_REG_ID]);
        append_code(codes, "\tcltq\n");
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[AX_REG_ID], QREG_NAMES[dst->reg_id]);
    }

    else if (src->suffix == SUFFIX_QUAD && SUFFIX_BYTE <= dst->suffix &&
             dst->suffix <= SUFFIX_QUAD) {
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[src->reg_id], QREG_NAMES[dst->reg_id]);
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
