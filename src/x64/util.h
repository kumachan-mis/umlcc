#ifndef _UMLCC_X64_UTIL_H_
#define _UMLCC_X64_UTIL_H_

#include "../vector/vector.h"

extern char* quad_regs[16];
extern char* double_regs[16];
extern int num_arg_reg;

char* stkptr_reg(char* regs[16]);
char* baseptr_reg(char* regs[16]);
char* arg_reg(char* regs[16], int index);
void append_code(Vector* codes, char* format, ...);

#endif
