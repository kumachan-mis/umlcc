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
    for (int i = 0; i < liveseqs_len; i++) {
        Liveseq* liveseq = vector_at(liveseqs, i);
        liveseq_goto_next(liveseq);
    }
}
