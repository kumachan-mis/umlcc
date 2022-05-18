#ifndef _UMLCC_LIVESEQ_H_
#define _UMLCC_LIVESEQ_H_

#include "../vector/vector.h"

typedef struct _Liveseq {
    Vector* livenesses;
    int liveness_index;
    int index;
} Liveseq;

Liveseq* new_liveseq(Vector* livenesses);
void delete_liveseq(Liveseq* liveseq);
int liveseq_need_evacuation(Liveseq* liveseq);
void liveseq_goto_next(Liveseq* liveseq);

#endif
