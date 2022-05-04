#ifndef _UMLCC_X64_H_
#define _UMLCC_X64_H_

#include "../vector/vector.h"

typedef struct _X64gen {
    Vector* _immcs;
} X64gen;

X64gen* new_x64gen(Vector* immcs);
void delete_x64gen(X64gen* x64gen);
Vector* x64gen_generate_x64code(X64gen* x64gen);

#endif
