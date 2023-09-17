#ifndef UMLCC_X64_H_PRIVATE
#define UMLCC_X64_H_PRIVATE
#endif

#include "./inst.h"
#include "../common/type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* x64_inst_formats[] = {"mov%c", "movs%c%c", "movz%c%c", "movabs%c", "lea%c", "c%ct%c", "c%ctd", "cqto",  "push%c",
                            "pop%c", "add%c",    "sub%c",    "imul%c",   "div%c", "idiv%c", "neg%c", "and%c", "not%c",
                            "or%c",  "xor%c",    "sal%c",    "sar%c",    "cmp%c", "sete",   "setne", "setl",  "setg",
                            "setle", "setge",    "jmp",      "je",       "jne",   "call",   "ret"};

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
    if (x64inst->src != NULL) {
        copied_x64inst->src = x64ope_copy(x64inst->src);
    }
    copied_x64inst->dst = NULL;
    if (x64inst->dst != NULL) {
        copied_x64inst->dst = x64ope_copy(x64inst->dst);
    }
    return copied_x64inst;
}

char* x64inst_tostring(X64Inst* x64inst) {
    char* x64instcode_tostring(X64Inst * x64inst);

    // TODO: fix to prevent buffer overflow
    char* code_str = malloc(200 * sizeof(char));
    memset(code_str, 0, 200 * sizeof(char));

    char* instcode_string = x64instcode_tostring(x64inst);
    strcat(code_str, "\t");
    strcat(code_str, instcode_string);
    free(instcode_string);

    int operand_appears = 0;
    if (x64inst->src != NULL && x64inst->src->type != X64_OPERAND_SUFFIX) {
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

char* x64instcode_tostring(X64Inst* x64inst) {
    char* inst_format = x64_inst_formats[x64inst->type];

    switch (x64inst->type) {
        // two-suffixes instruction
        case X64_INST_MOVSXX:
        case X64_INST_MOVZXX:
        case X64_INST_CXTX: {
            // TODO: fix to prevent buffer overflow
            char* instcode_str = malloc(50 * sizeof(char));
            char src_suffix = x64suffix_tochar(x64inst->src->suffix);
            char dst_suffix = x64suffix_tochar(x64inst->dst->suffix);
            sprintf(instcode_str, inst_format, src_suffix, dst_suffix);
            instcode_str = realloc(instcode_str, (strlen(instcode_str) + 1) * sizeof(char));
            return instcode_str;
        }

        // one-suffix instruction
        case X64_INST_MOVX:
        case X64_INST_MOVABSX:
        case X64_INST_LEAX:
        case X64_INST_CXTD:
        case X64_INST_PUSHX:
        case X64_INST_POPX:
        case X64_INST_ADDX:
        case X64_INST_SUBX:
        case X64_INST_IMULX:
        case X64_INST_DIVX:
        case X64_INST_IDIVX:
        case X64_INST_NEGX:
        case X64_INST_ANDX:
        case X64_INST_NOTX:
        case X64_INST_ORX:
        case X64_INST_XORX:
        case X64_INST_SALX:
        case X64_INST_SARX:
        case X64_INST_CMPX: {
            // TODO: fix to prevent buffer overflow
            char* instcode_str = malloc(50 * sizeof(char));
            char suffix = '\0';
            if (x64inst->src != NULL &&
                (x64inst->src->type == X64_OPERAND_SUFFIX || x64inst->src->type == X64_OPERAND_REG ||
                 x64inst->src->type == X64_OPERAND_INT)) {
                suffix = x64suffix_tochar(x64inst->src->suffix);
            }
            if (x64inst->dst != NULL &&
                (x64inst->dst->type == X64_OPERAND_SUFFIX || x64inst->dst->type == X64_OPERAND_REG ||
                 x64inst->dst->type == X64_OPERAND_INT)) {
                suffix = x64suffix_tochar(x64inst->dst->suffix);
            }
            sprintf(instcode_str, inst_format, suffix);
            instcode_str = realloc(instcode_str, (strlen(instcode_str) + 1) * sizeof(char));
            return instcode_str;
        }

        // zero-suffix instruction
        default:
            return new_string(inst_format);
    }
}

void delete_x64inst(X64Inst* x64inst) {
    if (x64inst->dst != NULL) {
        delete_x64ope(x64inst->dst);
    }
    if (x64inst->src != NULL) {
        delete_x64ope(x64inst->src);
    }
    free(x64inst);
}
