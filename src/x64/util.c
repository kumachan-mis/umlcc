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

void append_mov_code(Vector* codes, int src_reg_id, ImmcOpeSuffix src_immc_suffix, int dst_reg_id,
                     ImmcOpeSuffix dst_immc_suffix) {

    if (src_immc_suffix >= dst_immc_suffix) {
        if (src_reg_id == dst_reg_id) return;
        ImmcOpeSuffix immc_suffix = src_immc_suffix;
        char suffix = immcope_suffix_tochar(immc_suffix);
        char* src_name = reg_name(src_reg_id, immc_suffix);
        char* dst_name = reg_name(dst_reg_id, immc_suffix);
        append_code(codes, "\tmov%c\t%s, %s\n", suffix, src_name, dst_name);
        return;
    }

    if (src_immc_suffix == SUFFIX_LONG && dst_immc_suffix == SUFFIX_QUAD) {
        append_code(codes, "\tmovl\t%s, %s\n", LREG_NAMES[src_reg_id], LREG_NAMES[AX_REG_ID]);
        append_code(codes, "\tcltq\n");
        append_code(codes, "\tmovq\t%s, %s\n", QREG_NAMES[AX_REG_ID], QREG_NAMES[dst_reg_id]);
        return;
    }

    char src_suffix = immcope_suffix_tochar(src_immc_suffix);
    char dst_suffix = immcope_suffix_tochar(dst_immc_suffix);
    char* src_name = reg_name(src_reg_id, src_immc_suffix);
    char* dst_name = reg_name(dst_reg_id, dst_immc_suffix);
    append_code(codes, "\tmovs%c%c\t%s, %s\n", src_suffix, dst_suffix, src_name, dst_name);
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
