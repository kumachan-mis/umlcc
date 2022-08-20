#ifndef UMLCC_REGALLOC_REGALLOCRET_H
#define UMLCC_REGALLOC_REGALLOCRET_H

#include "../vector/vector.h"

typedef struct RegAllocReturn {
    Vector* immcs;
    Vector* liveseqs;
} RegAllocReturn;

RegAllocReturn* new_regallocret(Vector* immcs, Vector* liveseqs);
void regallocret_assign(Vector** immcs_ptr, Vector** liveseqs_ptr, RegAllocReturn* regallocret);

#endif
