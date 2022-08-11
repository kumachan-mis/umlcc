#ifndef UMLCC_REGALLOC_H
#define UMLCC_REGALLOC_H

#include "../vector/vector.h"
#include "./regallocret.h"

typedef struct RegAlloc {
    Vector* immcs;
    int immc_offset;
    int num_real_regs;
} RegAlloc;

RegAlloc* new_regalloc(Vector* immcs, int num_regs);
RegAllocReturn* regalloc_allocate_regs(RegAlloc* regalloc);
void delete_regalloc(RegAlloc* regalloc);

#endif
