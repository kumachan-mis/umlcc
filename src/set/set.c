#include "./set.h"
#include "../map/map.h"

#include <stdlib.h>

#define NOTNULL ((void*)1)

struct Set {
    Map* inner;
    HashableType* t_item;
};

struct SetIter {
    MapIter* inner;
};

void* copy_notnull() {
    return NOTNULL;
}

void delete_notnull() {}

BaseType t_notnull = {
    .copy_object = (void* (*)(void*))copy_notnull,
    .delete_object = (void (*)(void*))delete_notnull,
};

Set* new_set(HashableType* t_item) {
    Set* set = malloc(sizeof(Set));
    set->inner = new_map(t_item, &t_notnull);
    set->t_item = t_item;
    return set;
}

Set* set_copy(Set* set) {
    Set* copied_set = malloc(sizeof(Set));
    copied_set->t_item = set->t_item;
    copied_set->inner = map_copy(set->inner);
    return copied_set;
}

void set_add(Set* set, void* item) {
    map_add(set->inner, item, NOTNULL);
}

void set_remove(Set* set, void* item) {
    map_remove(set->inner, item);
}

int set_contains(Set* set, void* item) {
    return map_contains(set->inner, item);
}

SetIter* set_iter_begin(Set* set) {
    SetIter* iter = malloc(sizeof(SetIter));
    iter->inner = map_iter_begin(set->inner);
    return iter;
}

SetIter* set_iter_next(SetIter* iter, Set* set) {
    iter->inner = map_iter_next(iter->inner, set->inner);
    return iter;
}

int set_iter_end(SetIter* iter, Set* set) {
    int end = map_iter_end(iter->inner, set->inner);
    if (end) free(iter);
    return end;
}

void* set_iter_item(SetIter* iter, Set* set) {
    return map_iter_key(iter->inner, set->inner);
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

int set_issubset(Set* set, Set* other) {
    for (SetIter* iter = set_iter_begin(other); !set_iter_end(iter, other);
         iter = set_iter_next(iter, other)) {
        void* item = set_iter_item(iter, other);
        if (!set_contains(set, item)) return 0;
    }
    return 1;
}

int set_equals(Set* set, Set* other) {
    return set_issubset(set, other) && set_issubset(other, set);
}

void delete_set(Set* set) {
    delete_map(set->inner);
    free(set);
}
