#include "./set.h"
#include "../map/map.h"

#include <stdlib.h>

#define EXIST ((void*)1)

struct _Set {
    Map* inner;
    HashableType* t_item;
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
    set->inner = new_map(t_item, &t_exitst);
    set->t_item = t_item;
    return set;
}

void delete_set(Set* set) {
    delete_map(set->inner);
    free(set);
}

void set_add(Set* set, void* item) {
    map_add(set->inner, item, EXIST);
}

void set_remove(Set* set, void* item) {
    map_remove(set->inner, item);
}

int set_contains(Set* set, void* item) {
    return map_get(set->inner, item) == EXIST;
}

SetIter* set_iter_begin(Set* set) {
    return map_iter_begin(set->inner);
}

SetIter* set_iter_next(SetIter* iter, Set* set) {
    return map_iter_next(iter, set->inner);
}

int set_iter_end(SetIter* iter, Set* set) {
    return map_iter_end(iter, set->inner);
}

void* set_iter_item(SetIter* iter, Set* set) {
    return map_iter_key(iter, set->inner);
}

Set* set_intersection(Set* set, Set* other) {
    Set* intersection_set = new_set(set->t_item);
    for (SetIter* iter = set_iter_begin(set); !set_iter_end(iter, set);
         iter = set_iter_next(iter, set)) {
        void* item = set_iter_item(iter, set);
        if (!set_contains(other, item)) continue;
        set_add(intersection_set, set->t_item->copy_object(item));
    }
    return intersection_set;
}

Set* set_union(Set* set, Set* other) {
    Set* union_set = new_set(set->t_item);
    for (SetIter* iter = set_iter_begin(set); !set_iter_end(iter, set);
         iter = set_iter_next(iter, set)) {
        void* item = set_iter_item(iter, set);
        set_add(union_set, set->t_item->copy_object(item));
    }
    for (SetIter* iter = set_iter_begin(other); !set_iter_end(iter, other);
         iter = set_iter_next(iter, other)) {
        void* item = set_iter_item(iter, other);
        set_add(union_set, other->t_item->copy_object(item));
    }
    return union_set;
}

Set* set_difference(Set* set, Set* other) {
    Set* difference_set = new_set(set->t_item);
    for (SetIter* iter = set_iter_begin(set); !set_iter_end(iter, set);
         iter = set_iter_next(iter, set)) {
        void* item = set_iter_item(iter, set);
        if (set_contains(other, item)) continue;
        set_add(difference_set, set->t_item->copy_object(item));
    }
    return difference_set;
}
