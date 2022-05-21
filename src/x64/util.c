#include "./util.h"
#include "../liveseq/liveseq.h"

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

void liveseqs_next(Vector* liveseqs) {
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_id = 0; reg_id < liveseqs_len; reg_id++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_id);
        liveseq_goto_next(liveseq);
    }
}

Set* create_alive_regs_set(Vector* liveseqs) {
    Set* alive_regs_set = new_set(&t_integer);
    int liveseqs_len = vector_size(liveseqs);
    for (int reg_id = 0; reg_id < liveseqs_len; reg_id++) {
        Liveseq* liveseq = vector_at(liveseqs, reg_id);
        if (liveseq_is_alive(liveseq)) set_add(alive_regs_set, new_integer(reg_id));
    }
    return alive_regs_set;
}
