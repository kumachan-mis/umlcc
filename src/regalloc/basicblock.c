#include "./basicblock.h"
#include "../immc/immc.h"

#include <stdlib.h>

BasicBlock* new_basicblock(Vector* immcs) {
    BasicBlock* basicblock = malloc(sizeof(BasicBlock));
    basicblock->input_reg_ids = new_vector();
    basicblock->immcs = immcs;
    basicblock->output_reg_ids = new_vector();
    basicblock->next_block_ids = new_vector();
    return basicblock;
}

void delete_basicblock(BasicBlock* basicblock) {
    delete_vector(basicblock->input_reg_ids, free);
    delete_vector(basicblock->immcs, delete_immc);
    delete_vector(basicblock->output_reg_ids, free);
    delete_vector(basicblock->next_block_ids, free);
    free(basicblock);
}
