#ifndef _UMLCC_REGALLOC_BASICBLOCK_H_
#define _UMLCC_REGALLOC_BASICBLOCK_H_

#include "../vector/vector.h"
#include "../set/set.h"

typedef struct _BasicBlock {
    Set* input_reg_ids;
    Vector* immcs;
    Set* output_reg_ids;
    Set* succ_block_ids;
} BasicBlock;

extern BaseType t_basicblock;

BasicBlock* new_basicblock(Vector* immcs);
void delete_basicblock(BasicBlock* basicblock);

#endif
