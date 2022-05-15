#include "./liveness.h"

#include <stdlib.h>

BaseType t_reglivenes = {
    .copy_object = regliveness_copy,
    .delete_object = delete_regliveness,
};

RegLiveness* new_regliveness() {
    RegLiveness* regliveness = malloc(sizeof(RegLiveness));
    regliveness->first_def_index = -1;
    regliveness->last_use_index = -1;
    return regliveness;
}

void delete_regliveness(RegLiveness* regliveness) {
    free(regliveness);
}

RegLiveness* regliveness_copy(RegLiveness* regliveness) {
    RegLiveness* copied_regliveness = malloc(sizeof(RegLiveness));
    copied_regliveness->first_def_index = regliveness->first_def_index;
    copied_regliveness->last_use_index = regliveness->last_use_index;
    return copied_regliveness;
}
