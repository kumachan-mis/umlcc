#include "./set.h"
#include "../map/map.h"

#include <stdlib.h>

#define EXIST ((void*)1)

struct _Set {
    Map* _inner;
};

void delete_exist(void* exist);

Set* new_set() {
    Set* set = malloc(sizeof(Set));
    set->_inner = new_map();
    return set;
}

void delete_set(Set* set, void delete_item(void* item)) {
    delete_map(set->_inner, delete_item, delete_exist);
    free(set);
}

void set_add(Set* set, void* item, int hash_item(void* item, int max),
             int comp_item(void* a, void* b), void delete_item(void* item)) {
    map_add(set->_inner, item, EXIST, hash_item, comp_item, delete_item, delete_exist);
}

void set_remove(Set* set, void* item, int hash_item(void* item, int max),
                int comp_item(void* a, void* b), void delete_item(void* item)) {
    map_remove(set->_inner, item, hash_item, comp_item, delete_item, delete_exist);
}

int set_contains(Set* set, void* item, int hash_item(void* item, int max),
                 int comp_item(void* a, void* b)) {
    return map_get(set->_inner, item, hash_item, comp_item) == EXIST;
}

Set* set_intersection(Set* set, Set* other, int hash_item(void* item, int max),
                      int comp_item(void* a, void* b), void delete_item(void* item)) {}

Set* set_union(Set* set, Set* other, int hash_item(void* item, int max),
               int comp_item(void* a, void* b), void delete_item(void* item)) {}

Set* set_difference(Set* set, Set* other, int hash_item(void* item, int max),
                    int comp_item(void* a, void* b), void delete_item(void* item)) {}

void delete_exist(void* exist) {}
