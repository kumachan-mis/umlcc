#ifndef UMLCC_REGALLOC_BASICBLOCK_H
#define UMLCC_REGALLOC_BASICBLOCK_H

#include "../set/set.h"
#include "../vector/vector.h"

typedef struct BasicBlock {
    Vector* immcs;
    Set* input;
    Set* output;
    Set* succ;
} BasicBlock;

extern BaseType t_basicblock;

BasicBlock* new_basicblock(Vector* immcs);
BasicBlock* basicblock_copy(BasicBlock* basicblock);
void delete_basicblock(BasicBlock* basicblock);

#endif
