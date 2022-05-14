#include "./set.h"
#include "../map/map.h"

#include <stdlib.h>

#define EXIST ((void*)1)

struct _Set {
    Map* _inner;
};

struct _SetIter {
    int index;
};

void delete_exist() {}

BaseType t_exitst = {
    .delete_object = delete_exist,
};

Set* new_set(HashableType* t_item) {
    Set* set = malloc(sizeof(Set));
    set->_inner = new_map(t_item, &t_exitst);
    return set;
}

void delete_set(Set* set) {
    delete_map(set->_inner);
    free(set);
}

void set_add(Set* set, void* item) {
    map_add(set->_inner, item, EXIST);
}

void set_remove(Set* set, void* item) {
    map_remove(set->_inner, item);
}

int set_contains(Set* set, void* item) {
    return map_get(set->_inner, item) == EXIST;
}

SetIter* set_iter_begin(Set* set) {
    return map_iter_begin(set->_inner);
}

SetIter* set_iter_next(SetIter* iter, Set* set) {
    return map_iter_next(iter, set->_inner);
}

int set_iter_end(SetIter* iter, Set* set) {
    return map_iter_end(iter, set->_inner);
}

void* set_iter_item(SetIter* iter, Set* set) {
    return map_iter_key(iter, set->_inner);
}
