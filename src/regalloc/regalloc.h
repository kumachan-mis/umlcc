#ifndef _UMLCC_REGALLOC_H_
#define _UMLCC_REGALLOC_H_

#include "../vector/vector.h"
#include "./allocimmcs.h"

typedef struct _RegAlloc {
    Vector* _immcs;
    int _index;
    int _num_real_regs;
} RegAlloc;

RegAlloc* new_regalloc(Vector* immcs, int num_regs);
void delete_regalloc(RegAlloc* regalloc);
AllocImmcs* regalloc_allocate_regs(RegAlloc* regalloc);

#endif
