#include "./util.h"
#include "../immc/immc.h"
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

void append_conversion_code(Vector* codes, int reg_id, ImmcOpeSuffix from, ImmcOpeSuffix to) {
    if (from == SUFFIX_BYTE && to == SUFFIX_WORD) {
        append_code(codes, "\tmovsbw\t%s, %s\n", BREG_NAMES[reg_id], WREG_NAMES[reg_id]);
    } else if (from == SUFFIX_BYTE && to == SUFFIX_LONG) {
        append_code(codes, "\tmovsbl\t%s, %s\n", BREG_NAMES[reg_id], LREG_NAMES[reg_id]);
    } else if (from == SUFFIX_BYTE && to == SUFFIX_QUAD) {
        append_code(codes, "\tmovsbq\t%s, %s\n", BREG_NAMES[reg_id], QREG_NAMES[reg_id]);
    } else if (from == SUFFIX_WORD && to == SUFFIX_LONG) {
        append_code(codes, "\tmovswl\t%s, %s\n", WREG_NAMES[reg_id], LREG_NAMES[reg_id]);
    } else if (from == SUFFIX_WORD && to == SUFFIX_QUAD) {
        append_code(codes, "\tmovswq\t%s, %s\n", WREG_NAMES[reg_id], QREG_NAMES[reg_id]);
    } else if (from == SUFFIX_LONG && to == SUFFIX_QUAD) {
        append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[reg_id], LREG_NAMES[AX_REG_ID]);
        append_code(codes, "\tcltq\n");
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[AX_REG_ID], QREG_NAMES[reg_id]);
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
