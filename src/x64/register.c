#include "./register.h"

#include <stdlib.h>

int NUM_ARG_REGS = 6;
int ARG_REG_IDS[6] = {DI_REG_ID, SI_REG_ID, DX_REG_ID, CX_REG_ID, R8_REG_ID, R9_REG_ID};

int NUM_CALLER_SAVED_REGS = 8;
int CALLER_SAVED_REG_IDS[8] = {CX_REG_ID, DX_REG_ID, SI_REG_ID,  DI_REG_ID,
                               R8_REG_ID, R9_REG_ID, R10_REG_ID, R11_REG_ID};

int NUM_CALLEE_SAVED_REGS = 5;
int CALLEE_SAVED_REG_IDS[5] = {BX_REG_ID, BP_REG_ID, R12_REG_ID, R13_REG_ID, R14_REG_ID};

char* PC_NAME = "%rip";

char* BREG_NAMES[16] = {"%al",  "%bl",  "%cl",   "%dl",   "%spl",  "%bpl",  "%sil",  "%dil",
                        "%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", "%r14b", "%r15b"};

char* WREG_NAMES[16] = {"%ax",  "%bx",  "%cx",   "%dx",   "%sp",   "%bp",   "%si",   "%di",
                        "%r8w", "%r9w", "%r10w", "%r11w", "%r12w", "%r13q", "%r14w", "%r15w"};

char* LREG_NAMES[16] = {"%eax", "%ebx", "%ecx",  "%edx",  "%esp",  "%ebp",  "%esi",  "%edi",
                        "%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d"};

char* QREG_NAMES[16] = {"%rax", "%rbx", "%rcx", "%rdx", "%rsp", "%rbp", "%rsi", "%rdi",
                        "%r8",  "%r9",  "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

char* reg_name(X64RegisterId reg_id, X64Suffix suffix) {
    switch (suffix) {
        case X64_SUFFIX_BYTE:
            return BREG_NAMES[reg_id];
        case X64_SUFFIX_WORD:
            return WREG_NAMES[reg_id];
        case X64_SUFFIX_LONG:
            return LREG_NAMES[reg_id];
        case X64_SUFFIX_QUAD:
            return QREG_NAMES[reg_id];
        default:
            return NULL;
    }
}
