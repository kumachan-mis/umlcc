#include "./basicblock.h"
#include "../immc/immc.h"

#include <stdlib.h>

BasicBlock* new_basicblock(Vector* immcs) {
    BasicBlock* basicblock = malloc(sizeof(BasicBlock));
    basicblock->prev_block_ids = new_vector();
    basicblock->input_reg_ids = new_vector();
    basicblock->immcs = immcs;
    basicblock->output_reg_ids = new_vector();
    return basicblock;
}

void delete_basicblock(BasicBlock* basicblock) {
    delete_vector(basicblock->prev_block_ids, free);
    delete_vector(basicblock->input_reg_ids, free);
    delete_vector(basicblock->immcs, delete_immc);
    delete_vector(basicblock->output_reg_ids, free);
    free(basicblock);
}

void basicblock_add_prev_block_id(BasicBlock* basicblock, int prev_block_id) {

}

void basicblock_add_input_reg_id(BasicBlock* basicblock, int input_reg_id) {

}

void basicblock_add_output_reg_id(BasicBlock* basicblock, int output_reg_id) {

}
