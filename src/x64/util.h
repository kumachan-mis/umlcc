#ifndef _UMLCC_X64_UTIL_H_
#define _UMLCC_X64_UTIL_H_

#include "../vector/vector.h"

extern int num_arg_reg;
extern char* quad_regs[8];
extern char* double_regs[8];
char* arg_reg(char* regs[8], int index);
void append_code(Vector* codes, char* format, ...);

#endif
