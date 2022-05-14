#include "./basicblock.h"

#include <stdlib.h>

BaseType t_basicblock = {
    .delete_object = delete_basicblock,
};

BasicBlock* new_basicblock(Vector* immcs) {
    BasicBlock* basicblock = malloc(sizeof(BasicBlock));
    basicblock->input_reg_ids = new_vector(&t_hashable_integer);
    basicblock->immcs = immcs;
    basicblock->output_reg_ids = new_vector(&t_hashable_integer);
    basicblock->succ_block_ids = new_set(&t_hashable_integer);
    return basicblock;
}

void delete_basicblock(BasicBlock* basicblock) {
    delete_set(basicblock->input_reg_ids);
    delete_vector(basicblock->immcs);
    delete_set(basicblock->output_reg_ids);
    delete_set(basicblock->succ_block_ids);
    free(basicblock);
}
