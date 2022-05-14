#ifndef _UMLCC_REGALLOC_BASICBLOCK_H_
#define _UMLCC_REGALLOC_BASICBLOCK_H_

#include "../vector/vector.h"
#include "../set/set.h"

typedef struct _BasicBlock {
    Vector* immcs;
    Set* succ;
    Set* used;
    Set* defined;
    Set* input;
    Set* output;
} BasicBlock;

extern BaseType t_basicblock;

BasicBlock* new_basicblock(Vector* immcs);
void delete_basicblock(BasicBlock* basicblock);

#endif
