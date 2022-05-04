#include "./register.h"

#include <stdlib.h>

char* quad_regs[16] = {"%rax", "%rbx", "%rcx", "%rdx", "%rsp", "%rbp", "%rsi", "%rdi",
                       "%r8",  "%r9",  "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

char* double_regs[16] = {"%eax", "%ebx", "%ecx",  "%edx",  "%esp",  "%ebp",  "%esi",  "%edi",
                         "%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d"};

// If the class is INTEGER, the next available register of the sequence
// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
// cf. System V Application Binary Interface (p20)
//     https://uclibc.org/docs/psABI-x86_64.pdf

int num_arg_regs = 6;
int num_caller_saved_regs = 8;
int num_callee_saved_regs = 5;

char* acc_reg(char* regs[16]) {
    return regs[0];
}

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

char* caller_saved_reg(char* regs[16], int index) {
    switch (index) {
        case 0:
            return regs[2];
        case 1:
            return regs[3];
        case 2:
            return regs[6];
        case 3:
            return regs[7];
        case 4:
            return regs[8];
        case 5:
            return regs[9];
        case 6:
            return regs[10];
        case 7:
            return regs[11];
        default:
            return NULL;
    }
}

char* callee_saved_reg(char* regs[16], int index) {
    switch (index) {
        case 0:
            return regs[1];
        case 1:
            return regs[12];
        case 2:
            return regs[13];
        case 3:
            return regs[14];
        case 4:
            return regs[15];
        default:
            return NULL;
    }
}

int* new_caller_saved_map() {
    int* caller_saved_map = malloc(num_caller_saved_regs * sizeof(int));
    for (int i = 0; i < num_caller_saved_regs; i++) {
        caller_saved_map[i] = -1;
    }
    return caller_saved_map;
}

void delete_caller_saved_map(int* caller_saved_map) {
    free(caller_saved_map);
}

int caller_saved_map_allocate(int* caller_saved_map, int virtual_reg_name) {
    for (int reg_name = 0; reg_name < num_caller_saved_regs; reg_name++) {
        if (caller_saved_map[reg_name] == -1) {
            caller_saved_map[reg_name] = virtual_reg_name;
            return reg_name;
        }
    }
    return -1;
}

int caller_saved_map_search(int* caller_saved_map, int virtual_reg_name) {
    for (int reg_name = 0; reg_name < num_caller_saved_regs; reg_name++) {
        if (caller_saved_map[reg_name] == virtual_reg_name) return reg_name;
    }
    return -1;
}

void caller_saved_map_free(int* caller_saved_map, int virtual_reg_name) {
    for (int reg_name = 0; reg_name < num_caller_saved_regs; reg_name++) {
        if (caller_saved_map[reg_name] == virtual_reg_name) {
            caller_saved_map[reg_name] = -1;
            break;
        }
    }
}
