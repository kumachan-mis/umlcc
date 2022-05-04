#ifndef _UMLCC_X64_REGISTER_H_
#define _UMLCC_X64_REGISTER_H_

extern char* quad_regs[16];
extern char* double_regs[16];
extern int num_arg_regs;
extern int num_caller_saved_regs;
extern int num_callee_saved_regs;

char* stkptr_reg(char* regs[16]);
char* baseptr_reg(char* regs[16]);
char* arg_reg(char* regs[16], int index);
char* caller_saved_reg(char* regs[16], int index);
char* callee_saved_reg(char* regs[16], int index);

int* new_caller_saved_map();
void delete_caller_saved_map(int* caller_saved_map);
int caller_saved_map_allocate(int* caller_saved_map, int virtual_reg_name);
int caller_saved_map_search(int* caller_saved_map, int virtual_reg_name);
void caller_saved_map_free(int* caller_saved_map, int virtual_reg_name);

#endif
