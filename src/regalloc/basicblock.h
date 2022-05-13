#ifndef _UMLCC_REGALLOC_BASICBLOCK_H_
#define _UMLCC_REGALLOC_BASICBLOCK_H_

#include "../vector/vector.h"

typedef struct _BasicBlock {
    Vector* prev_block_ids;
    Vector* input_reg_ids;
    Vector* immcs;
    Vector* output_reg_ids;
} BasicBlock;

BasicBlock* new_basicblock(Vector* immcs);
void delete_basicblock(BasicBlock* basicblock);
void basicblock_add_prev_block_id(BasicBlock* basicblock, int prev_block_id);
void basicblock_add_input_reg_id(BasicBlock* basicblock, int input_reg_id);
void basicblock_add_output_reg_id(BasicBlock* basicblock, int output_reg_id);


#endif
