#include "./basicblock.h"

#include <stdlib.h>

BaseType t_basicblock = {
    .delete_object = delete_basicblock,
};

BasicBlock* new_basicblock(Vector* immcs) {
    BasicBlock* basicblock = malloc(sizeof(BasicBlock));
    basicblock->immcs = immcs;
    basicblock->input = new_vector(&t_hashable_integer);
    basicblock->output = new_vector(&t_hashable_integer);
    basicblock->succ = new_set(&t_hashable_integer);
    return basicblock;
}

void delete_basicblock(BasicBlock* basicblock) {
    delete_vector(basicblock->immcs);
    delete_set(basicblock->input);
    delete_set(basicblock->output);
    delete_set(basicblock->succ);
    free(basicblock);
}
