#include "./liveness.h"

#include <stdlib.h>

BaseType t_liveness = {
    .copy_object = liveness_copy,
    .delete_object = delete_liveness,
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

int liveness_isinit(Liveness* liveness) {
    return liveness->first_def_index == -1 && liveness->last_use_index == -1;
}

void delete_liveness(Liveness* liveness) {
    free(liveness);
}
