#ifndef _UMLCC_X64_REGISTER_H_
#define _UMLCC_X64_REGISTER_H_

extern char* pc_name;
extern char* quad_regs[16];
extern char* double_regs[16];
extern int num_arg_regs;

char* stackptr_reg(char* regs[16]);
char* baseptr_reg(char* regs[16]);
char* arg_reg(char* regs[16], int index);
char* callee_saved_reg(char* regs[16], int index);

#endif
