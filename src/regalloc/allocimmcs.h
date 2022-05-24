#ifndef UMLCC_REGALLOC_ALLOCIMMCS_H
#define UMLCC_REGALLOC_ALLOCIMMCS_H

#include "../vector/vector.h"

typedef struct AllocImmcs {
    Vector* immcs;
    Vector* liveseqs;
} AllocImmcs;

AllocImmcs* new_allocimmcs(Vector* immcs, Vector* liveseqs);
void allocimmcs_move(AllocImmcs* allocimmcs, Vector** allocated_immcs_ref, Vector** liveseqs_ref);

#endif
