#include "./lifetime.h"

#include <stdlib.h>

Lifetime* new_lifetime(int firstdef_index, int lastuse_index) {
    Lifetime* lifetime = malloc(sizeof(Lifetime));
    lifetime->firstdef_index = firstdef_index;
    lifetime->lastuse_index = lastuse_index;
}

void delete_lifetime(Lifetime* lifetime) {
    free(lifetime);
}