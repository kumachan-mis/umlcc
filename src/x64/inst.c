#include "./inst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* x64_inst_formats[] = {"mov%c", "movs%c%c", "lea%c",  "c%ct%c", "c%ctd", "push%c", "pop%c",
                            "add%c", "sub%c",    "imul%c", "idiv%c", "sal%c", "sar%c",  "cmp%c",
                            "sete",  "setne",    "jmp",    "je",     "jne",   "call"};

X64Inst* new_x64inst(X64InstType type, X64Ope* dst, X64Ope* src) {
    X64Inst* x64inst = malloc(sizeof(X64Inst));
    x64inst->type = type;
    x64inst->dst = dst;
    x64inst->src = src;
    return x64inst;
}

X64Inst* x64inst_copy(X64Inst* x64inst) {
    X64Inst* copied_x64inst = malloc(sizeof(X64Inst));
    copied_x64inst->type = x64inst->type;
    copied_x64inst->dst = NULL;
    copied_x64inst->src = NULL;
    if (x64inst->dst != NULL) copied_x64inst->dst = x64ope_copy(x64inst->dst);
    if (x64inst->src != NULL) copied_x64inst->src = x64ope_copy(x64inst->src);
    return copied_x64inst;
}

char* x64inst_tostring(X64Inst* x64inst) {
    char* code_str = malloc(100 * sizeof(char));
    memset(code_str, 0, 100 * sizeof(char));

    char primary_suffix = x64suffix_tochar(x64inst->dst->suffix);
    char secondary_suffix = x64suffix_tochar(x64inst->dst->suffix);
    strcat(code_str, "\t");
    sprintf(code_str + 1, x64_inst_formats[x64inst->type], primary_suffix, secondary_suffix);

    int operand_appears = 0;
    if (x64inst->dst != NULL) {
        char* dst_string = x64ope_tostring(x64inst->dst);
        strcat(code_str, "\t");
        strcat(code_str, dst_string);
        free(dst_string);
        operand_appears = 1;
    }

    if (x64inst->src != NULL) {
        char* src_string = x64ope_tostring(x64inst->src);
        strcat(code_str, operand_appears ? ", " : "\t");
        strcat(code_str, src_string);
        free(src_string);
        operand_appears = 1;
    }

    strcat(code_str, "\n");
    code_str = realloc(code_str, (strlen(code_str) + 1) * sizeof(char));
    return code_str;
}

void delete_x64inst(X64Inst* x64inst) {
    if (x64inst->dst != NULL) delete_x64ope(x64inst->dst);
    if (x64inst->src != NULL) delete_x64ope(x64inst->src);
    free(x64inst);
}
