#include "./liveness.h"

#include <stdlib.h>

BaseType t_liveness = {
    .copy_object = (void* (*)(void*))liveness_copy,
    .delete_object = (void (*)(void*))delete_liveness,
};

Liveness* new_liveness(int immc_index) {
    Liveness* liveness = malloc(sizeof(Liveness));
    liveness->first_def_index = immc_index;
    liveness->last_use_index = immc_index;
    return liveness;
}

Liveness* liveness_copy(Liveness* liveness) {
    Liveness* copied_liveness = malloc(sizeof(Liveness));
    copied_liveness->first_def_index = liveness->first_def_index;
    copied_liveness->last_use_index = liveness->last_use_index;
    return copied_liveness;
}

void delete_liveness(Liveness* liveness) {
    free(liveness);
}
