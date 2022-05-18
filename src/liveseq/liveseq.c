#include "./liveseq.h"
#include "./liveness.h"

#include <stdlib.h>

Liveseq* new_liveseq(Vector* livenesses) {
    Liveseq* liveseq = malloc(sizeof(Liveseq));
    liveseq->livenesses = livenesses;
    liveseq->index = 0;

    int livenesses_len = vector_size(liveseq->livenesses);
    if (livenesses_len == 0) {
        liveseq->liveness_index = -1;
        return liveseq;
    }

    Liveness* first_liveness = vector_at(liveseq->livenesses, 0);
    if (liveseq->index < first_liveness->first_def_index) {
        liveseq->liveness_index = -1;
        return liveseq;
    }

    liveseq->liveness_index = 0;
    return liveseq;
}

void delete_liveseq(Liveseq* liveseq) {
    delete_vector(liveseq->livenesses);
    free(liveseq);
}

int liveseq_need_evacuation(Liveseq* liveseq) {
    int livenesses_len = vector_size(liveseq->livenesses);
    if (livenesses_len == 0 || liveseq->liveness_index < 0 ||
        liveseq->liveness_index >= livenesses_len) {
        return 0;
    }

    int index = liveseq->index;
    Liveness* liveness = vector_at(liveseq->livenesses, liveseq->liveness_index);
    int need_evacuation = liveness->first_def_index <= index && index < liveness->last_use_index;

    if (liveseq->liveness_index < livenesses_len - 1) {
        Liveness* next_liveness = vector_at(liveseq->livenesses, liveseq->liveness_index + 1);
        need_evacuation = need_evacuation || next_liveness->first_def_index <= index;
    }

    return need_evacuation;
}

void liveseq_goto_next(Liveseq* liveseq) {
    liveseq->index++;
    int index = liveseq->index;

    int livenesses_len = vector_size(liveseq->livenesses);
    if (livenesses_len == 0 || liveseq->liveness_index >= livenesses_len) return;

    if (liveseq->liveness_index < 0) {
        Liveness* first_liveness = vector_at(liveseq->livenesses, 0);
        if (index >= first_liveness->first_def_index) liveseq->liveness_index = 0;
        return;
    }

    Liveness* liveness = vector_at(liveseq->livenesses, liveseq->liveness_index);
    if (liveseq->liveness_index == livenesses_len - 1) {
        if (index >= liveness->last_use_index) liveseq->liveness_index++;
        return;
    }

    Liveness* next_liveness = vector_at(liveseq->livenesses, liveseq->liveness_index + 1);
    if (index >= next_liveness->first_def_index) liveseq->liveness_index++;
}
