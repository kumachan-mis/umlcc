#include "./basicblock.h"

#include <stdlib.h>

BaseType t_basicblock = {
    .copy_object = (void* (*)(void*))basicblock_copy,
    .delete_object = (void (*)(void*))delete_basicblock,
};

BasicBlock* new_basicblock(Vector* immcs) {
    BasicBlock* basicblock = malloc(sizeof(BasicBlock));
    basicblock->immcs = immcs;
    basicblock->input = new_set(&t_hashable_integer);
    basicblock->output = new_set(&t_hashable_integer);
    basicblock->succ = new_set(&t_hashable_integer);
    return basicblock;
}

BasicBlock* basicblock_copy(BasicBlock* basicblock) {
    BasicBlock* copied_basicblock = malloc(sizeof(BasicBlock));
    copied_basicblock->immcs = vector_copy(basicblock->immcs);
    copied_basicblock->input = set_copy(basicblock->input);
    copied_basicblock->output = set_copy(basicblock->output);
    copied_basicblock->succ = set_copy(basicblock->succ);
    return copied_basicblock;
}

void delete_basicblock(BasicBlock* basicblock) {
    delete_vector(basicblock->immcs);
    delete_set(basicblock->input);
    delete_set(basicblock->output);
    delete_set(basicblock->succ);
    free(basicblock);
}
