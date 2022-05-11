#ifndef _UMLCC_REGALLOC_BASICBLOCK_H_
#define _UMLCC_REGALLOC_BASICBLOCK_H_

#include "../vector/vector.h"

typedef struct _BasicBlock {
    Vector* input_reg_ids;
    Vector* immcs;
    Vector* output_reg_ids;
    Vector* next_block_ids;
} BasicBlock;

BasicBlock* new_basicblock(Vector* immcs);
void delete_basicblock(BasicBlock* basicblock);

#endif
