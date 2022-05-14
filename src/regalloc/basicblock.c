#include "./basicblock.h"

#include <stdlib.h>

BaseType t_basicblock = {
    .delete_object = delete_basicblock,
};

BasicBlock* new_basicblock(Vector* immcs) {
    BasicBlock* basicblock = malloc(sizeof(BasicBlock));
    basicblock->immcs = immcs;
    basicblock->succ = new_set(&t_hashable_integer);
    basicblock->used = new_set(&t_hashable_integer);
    basicblock->defined = new_set(&t_hashable_integer);
    basicblock->input = new_vector(&t_hashable_integer);
    basicblock->output = new_vector(&t_hashable_integer);
    return basicblock;
}

void delete_basicblock(BasicBlock* basicblock) {
    delete_vector(basicblock->immcs);
    delete_set(basicblock->succ);
    delete_set(basicblock->used);
    delete_set(basicblock->defined);
    delete_set(basicblock->input);
    delete_set(basicblock->output);
    free(basicblock);
}
