#ifndef UMLCC_LIVESEQ_H
#define UMLCC_LIVESEQ_H

#include "../common/type.h"
#include "../vector/vector.h"

typedef struct Liveseq {
    Vector* livenesses;
    int liveness_index;
    int index;
} Liveseq;

extern BaseType t_liveseq;

Liveseq* new_liveseq(void);
Liveseq* liveseq_copy(Liveseq* liveseq);
int liveseq_isalive(Liveseq* liveseq);
void liveseq_goto_next(Liveseq* liveseq);
void delete_liveseq(Liveseq* liveseq);

#endif
