#ifndef _UMLCC_REGALLOC_H_
#define _UMLCC_REGALLOC_H_

#include "../vector/vector.h"

typedef struct _RegAlloc {
    Vector* _immcs;
    int _index;
    int _num_regs;
} RegAlloc;

RegAlloc* new_regalloc(Vector* immcs, int num_regs);
void delete_regalloc(RegAlloc* regalloc);
Vector* regalloc_allocate_regs(RegAlloc* regalloc);

#endif
