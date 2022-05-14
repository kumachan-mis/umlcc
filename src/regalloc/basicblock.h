#ifndef _UMLCC_REGALLOC_BASICBLOCK_H_
#define _UMLCC_REGALLOC_BASICBLOCK_H_

#include "../set/set.h"
#include "../vector/vector.h"

typedef struct _BasicBlock {
    Vector* immcs;
    Set* input;
    Set* output;
    Set* succ;
} BasicBlock;

extern BaseType t_basicblock;

BasicBlock* new_basicblock(Vector* immcs);
void delete_basicblock(BasicBlock* basicblock);

#endif
