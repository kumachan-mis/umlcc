#include "./inst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* x64_inst_formats[] = {"mov%c", "movs%c%c", "lea%c",  "c%ct%c", "c%ctd", "push%c", "pop%c",
                            "add%c", "sub%c",    "imul%c", "idiv%c", "sal%c", "sar%c",  "cmp%c",
                            "sete",  "setne",    "jmp",    "je",     "jne",   "call",   "ret"};

X64Inst* new_x64inst(X64InstType type, X64Ope* src, X64Ope* dst) {
    X64Inst* x64inst = malloc(sizeof(X64Inst));
    x64inst->type = type;
    x64inst->src = src;
    x64inst->dst = dst;
    return x64inst;
}

X64Inst* x64inst_copy(X64Inst* x64inst) {
    X64Inst* copied_x64inst = malloc(sizeof(X64Inst));
    copied_x64inst->type = x64inst->type;
    copied_x64inst->src = NULL;
    copied_x64inst->dst = NULL;
    if (x64inst->src != NULL) copied_x64inst->src = x64ope_copy(x64inst->src);
    if (x64inst->dst != NULL) copied_x64inst->dst = x64ope_copy(x64inst->dst);
    return copied_x64inst;
}

char* x64inst_tostring(X64Inst* x64inst) {
    char* code_str = malloc(100 * sizeof(char));
    memset(code_str, 0, 100 * sizeof(char));

    strcat(code_str, "\t");

    if (x64inst->src != NULL && x64inst->dst != NULL) {
        char src_suffix = x64suffix_tochar(x64inst->src->suffix);
        char dst_suffix = x64suffix_tochar(x64inst->dst->suffix);
        sprintf(code_str + 1, x64_inst_formats[x64inst->type], src_suffix, dst_suffix);
    } else if (x64inst->src != NULL) {
        char src_suffix = x64suffix_tochar(x64inst->src->suffix);
        sprintf(code_str + 1, x64_inst_formats[x64inst->type], src_suffix);
    } else if (x64inst->dst != NULL) {
        char dst_suffix = x64suffix_tochar(x64inst->dst->suffix);
        sprintf(code_str + 1, x64_inst_formats[x64inst->type], dst_suffix);
    } else {
        strcat(code_str + 1, x64_inst_formats[x64inst->type]);
    }

    int operand_appears = 0;
    if (x64inst->src != NULL && x64inst->dst->type != X64_OPERAND_SUFFIX) {
        char* src_string = x64ope_tostring(x64inst->src);
        strcat(code_str, "\t");
        strcat(code_str, src_string);
        free(src_string);
        operand_appears = 1;
    }

    if (x64inst->dst != NULL && x64inst->dst->type != X64_OPERAND_SUFFIX) {
        char* dst_string = x64ope_tostring(x64inst->dst);
        strcat(code_str, operand_appears ? ", " : "\t");
        strcat(code_str, dst_string);
        free(dst_string);
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
