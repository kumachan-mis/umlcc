#ifndef UMLCC_X64_H
#define UMLCC_X64_H

#include "../vector/vector.h"
#include "./register.h"

typedef struct X64gen {
    Vector* immcs;
    Vector* liveseqs;
    int index;
    int evacuation_count;
} X64gen;

X64gen* new_x64gen(Vector* immcs, Vector* liveseqs);
Vector* x64gen_generate_x64code(X64gen* x64gen);
void delete_x64gen(X64gen* x64gen);

#endif
