#include "./liveseq.h"
#include "./liveness.h"

#include <stdlib.h>

BaseType t_liveseq = {
    .copy_object = (void* (*)(void*))liveseq_copy,
    .delete_object = (void (*)(void*))delete_liveseq,
};

Liveseq* new_liveseq() {
    Liveseq* liveseq = malloc(sizeof(Liveseq));
    liveseq->livenesses = new_vector(&t_liveness);
    liveseq->index = 0;
    liveseq->liveness_index = -1;
    return liveseq;
}

Liveseq* liveseq_copy(Liveseq* liveseq) {
    Liveseq* copied_liveseq = malloc(sizeof(Liveseq));
    copied_liveseq->livenesses = vector_copy(liveseq->livenesses);
    copied_liveseq->index = liveseq->index;
    copied_liveseq->liveness_index = liveseq->liveness_index;
    return copied_liveseq;
}

int liveseq_is_alive(Liveseq* liveseq) {
    // following registers are NOT alive:
    // - registers to be first-defined at current index
    // - registers which are last-used at current index

    int index = liveseq->index;

    int livenesses_len = vector_size(liveseq->livenesses);
    if (livenesses_len == 0 || liveseq->liveness_index >= livenesses_len) return 0;

    if (liveseq->liveness_index < 0) {
        Liveness* first_liveness = vector_at(liveseq->livenesses, 0);
        return index > first_liveness->first_def_index;
    }

    Liveness* liveness = vector_at(liveseq->livenesses, liveseq->liveness_index);
    return liveness->first_def_index < index && index < liveness->last_use_index;
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
        if (index >= liveness->last_use_index) liveseq->liveness_index = livenesses_len;
        return;
    }

    Liveness* next_liveness = vector_at(liveseq->livenesses, liveseq->liveness_index + 1);
    if (index >= next_liveness->first_def_index) liveseq->liveness_index++;
}

void delete_liveseq(Liveseq* liveseq) {
    delete_vector(liveseq->livenesses);
    free(liveseq);
}
