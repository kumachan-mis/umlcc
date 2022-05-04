#include "./x64.h"
#include "../immc/immc.h"

#include <stdlib.h>

X64gen* new_x64gen(Vector* immcs) {
    X64gen* x64gen = malloc(sizeof(X64gen));
    x64gen->_immcs = immcs;
    return x64gen;
}

void delete_x64gen(X64gen* x64gen) {
    delete_vector(x64gen->_immcs, (void (*)(void* item))delete_immc);
    free(x64gen);
}

Vector* x64gen_generate_x64code(X64gen* x64gen) {
    return NULL;
}
