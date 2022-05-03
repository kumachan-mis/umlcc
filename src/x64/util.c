#include "./util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* quad_regs[16] = {"%rax", "%rbx", "%rcx", "%rdx", "%rsp", "%rbp", "%rsi", "%rdi",
                       "%r8",  "%r9",  "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

char* double_regs[16] = {"%eax", "%ebx", "%ecx",  "%edx",  "%esp",  "%ebp",  "%esi",  "%edi",
                         "%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d"};

// If the class is INTEGER, the next available register of the sequence
// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
// cf. System V Application Binary Interface (p20)
//     https://uclibc.org/docs/psABI-x86_64.pdf

int num_arg_reg = 6;

char* stkptr_reg(char* regs[16]) {
    return regs[4];
}

char* baseptr_reg(char* regs[16]) {
    return regs[5];
}

char* arg_reg(char* regs[16], int index) {
    switch (index) {
        case 0:
            return regs[7];
        case 1:
            return regs[6];
        case 2:
            return regs[3];
        case 3:
            return regs[2];
        case 4:
            return regs[8];
        case 5:
            return regs[9];
        default:
            return NULL;
    }
}

void append_code(Vector* codes, char* format, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, format);

    char* code = malloc((100 + 1) * sizeof(char));
    vsprintf(code, format, arg_ptr);
    code = realloc(code, (strlen(code) + 1) * sizeof(char));
    vector_push(codes, code);

    va_end(arg_ptr);
}
