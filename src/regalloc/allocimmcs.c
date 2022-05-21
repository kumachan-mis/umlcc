#include "./allocimmcs.h"

#include <stdlib.h>

AllocImmcs* new_allocimmcs(Vector* immcs, Vector* liveseqs) {
    AllocImmcs* allocimmcs = malloc(sizeof(AllocImmcs));
    allocimmcs->immcs = immcs;
    allocimmcs->liveseqs = liveseqs;
    return allocimmcs;
}

void allocimmcs_move(AllocImmcs* allocimmcs, Vector** allocated_immcs_ref, Vector** liveseqs_ref) {
    *allocated_immcs_ref = allocimmcs->immcs;
    *liveseqs_ref = allocimmcs->liveseqs;
    free(allocimmcs);
}
