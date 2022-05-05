#include "./consts.h"

#include <stdlib.h>

char* PC_NAME = "%rip";

char* QREG_NAMES[16] = {"%rax", "%rbx", "%rcx", "%rdx", "%rsp", "%rbp", "%rsi", "%rdi",
                        "%r8",  "%r9",  "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

char* LREG_NAMES[16] = {"%eax", "%ebx", "%ecx",  "%edx",  "%esp",  "%ebp",  "%esi",  "%edi",
                        "%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d"};

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

int NUM_ARGREGS = 6;

// If the class is INTEGER, the next available register of the sequence
// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
// cf. System V Application Binary Interface (p20)
//     https://uclibc.org/docs/psABI-x86_64.pdf

char* arg_reg_name(char* reg_names[16], int index) {
    switch (index) {
        case 0:
            return reg_names[7];
        case 1:
            return reg_names[6];
        case 2:
            return reg_names[3];
        case 3:
            return reg_names[2];
        case 4:
            return reg_names[8];
        case 5:
            return reg_names[9];
        default:
            return NULL;
    }
}

char* callee_reg_name(char* reg_names[16], int index) {
    switch (index) {
        case 0:
            return reg_names[1];
        case 1:
            return reg_names[12];
        case 2:
            return reg_names[13];
        case 3:
            return reg_names[14];
        case 4:
            return reg_names[15];
        default:
            return NULL;
    }
}
