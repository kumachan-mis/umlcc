#ifndef _UMLCC_X64_REGALLOC_H_
#define _UMLCC_X64_REGALLOC_H_

#include "../vector/vector.h"

typedef struct _RegAlloc {
    int* _allocation_table;
} RegAlloc;

typedef struct _RegEvacuationEntry {
    int caller_saved_index;
    int callee_saved_index;
} RegEvacuationEntry;

RegAlloc* new_regalloc();
void delete_regalloc(RegAlloc* regalloc);

int regalloc_allocate(RegAlloc* regalloc, int virtual_reg_id);
int regalloc_force_allocate(RegAlloc* regalloc, int virtual_reg_id, int real_reg_id);
int regalloc_realloc(RegAlloc* regalloc, int virtual_reg_id);
int regalloc_resolve(RegAlloc* regalloc, int virtual_reg_id);
void regalloc_free(RegAlloc* regalloc, int virtual_reg_id);

int regalloc_lock(RegAlloc* regalloc);
int regalloc_force_lock(RegAlloc* regalloc, int real_reg_id);
int regalloc_inuse(RegAlloc* regalloc, int real_reg_id);
int regalloc_unlock(RegAlloc* regalloc, int real_reg_id);

Vector* regalloc_evacuate(RegAlloc* regalloc);
void regalloc_restore(RegAlloc* regalloc, Vector* evacuation_table);

#endif
