#include "./register.h"

#include <stdlib.h>

char* PC_NAME = "%rip";

char* BREG_NAMES[16] = {"%al",  "%bl",  "%cl",   "%dl",   "%spl",  "%bpl",  "%sil",  "%dil",
                        "%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", "%r14b", "%r15b"};

char* WREG_NAMES[16] = {"%ax",  "%bx",  "%cx",   "%dx",   "%sp",   "%bp",   "%si",   "%di",
                        "%r8w", "%r9w", "%r10w", "%r11w", "%r12w", "%r13q", "%r14w", "%r15w"};

char* LREG_NAMES[16] = {"%eax", "%ebx", "%ecx",  "%edx",  "%esp",  "%ebp",  "%esi",  "%edi",
                        "%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d"};

char* QREG_NAMES[16] = {"%rax", "%rbx", "%rcx", "%rdx", "%rsp", "%rbp", "%rsi", "%rdi",
                        "%r8",  "%r9",  "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

// If the class is INTEGER, the next available register of the sequence
// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
// cf. System V Application Binary Interface (p20)
//     https://uclibc.org/docs/psABI-x86_64.pdf
int ARG_REG_IDS[6] = {7, 6, 3, 2, 8, 9};

int CALLER_SAVED_REG_IDS[8] = {2, 3, 6, 7, 8, 9, 10, 11};

int CALLEE_SAVED_REG_IDS[5] = {1, 12, 13, 14, 15};

int AX_REG_ID = 0;
int BX_REG_ID = 1;
int CX_REG_ID = 2;
int DX_REG_ID = 3;
int SP_REG_ID = 4;
int BP_REG_ID = 5;
int SI_REG_ID = 6;
int DI_REG_ID = 7;
int R8_REG_ID = 8;
int R9_REG_ID = 9;
int R10_REG_ID = 10;
int R11_REG_ID = 11;
int R12_REG_ID = 12;
int R13_REG_ID = 13;
int R14_REG_ID = 14;
int R15_REG_ID = 15;

int NUM_REGS = 16;
int NUM_ARG_REGS = 6;
int NUM_CALLER_SAVED_REGS = 8;
int NUM_CALLEE_SAVED_REGS = 5;

char* reg_name(int reg_id, ImmcOpeSuffix suffix) {
    switch (suffix) {
        case SUFFIX_BYTE:
            return BREG_NAMES[reg_id];
        case SUFFIX_WORD:
            return WREG_NAMES[reg_id];
        case SUFFIX_LONG:
            return LREG_NAMES[reg_id];
        case SUFFIX_QUAD:
            return QREG_NAMES[reg_id];
        default:
            return NULL;
    }
}
