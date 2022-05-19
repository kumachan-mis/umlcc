#ifndef _UMLCC_LIVESEQ_H_
#define _UMLCC_LIVESEQ_H_

#include "../common/common.h"
#include "../vector/vector.h"

typedef struct _Liveseq {
    Vector* livenesses;
    int liveness_index;
    int index;
} Liveseq;

extern BaseType t_liveseq;

Liveseq* new_liveseq();
void delete_liveseq(Liveseq* liveseq);
Liveseq* liveseq_copy(Liveseq* liveseq);
int liveseq_need_evacuation(Liveseq* liveseq);
void liveseq_goto_next(Liveseq* liveseq);

#endif
