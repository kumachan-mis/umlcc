#ifndef _UMLCC_X64_REGALLOC_H_
#define _UMLCC_X64_REGALLOC_H_

int* new_regalloc();
void delete_regalloc(int* regalloc);
int regalloc_allocate_caller_saved(int* regalloc, int virtual_reg_id);
int regalloc_allocate_callee_saved(int* regalloc, int virtual_reg_id);
int regalloc_force_allocate(int* regalloc, int virtual_reg_id, int real_reg_id);
int regalloc_change_caller_saved_allocation(int* regalloc, int virtual_reg_id);
void regalloc_free(int* regalloc, int virtual_reg_id);
int regalloc_search(int* regalloc, int virtual_reg_id);
int regalloc_is_allocated(int* regalloc, int real_reg_id);

#endif
