#include "./pair.h"

#include <stdlib.h>

struct Pair {
    BaseType* t_first;
    BaseType* t_second;
    void* first;
    void* second;
};

BaseType t_pair = {
    .copy_object = (void* (*)(void*))pair_copy,
    .delete_object = (void (*)(void*))delete_pair,
};

Pair* new_pair(BaseType* t_first, BaseType* t_second) {
    Pair* pair = malloc(sizeof(Pair));
    pair->t_first = t_first;
    pair->t_second = t_second;
    pair->first = NULL;
    pair->second = NULL;
    return pair;
}

Pair* pair_copy(Pair* pair) {
    Pair* copied_pair = malloc(sizeof(Pair));
    copied_pair->t_first = pair->t_first;
    copied_pair->t_second = pair->t_second;
    copied_pair->first = NULL;
    if (pair->first != NULL) {
        copied_pair->first = pair->t_first->copy_object(pair->first);
    }
    copied_pair->second = NULL;
    if (pair->second != NULL) {
        copied_pair->second = pair->t_second->copy_object(pair->second);
    }
    return copied_pair;
}

void* pair_first(Pair* pair) {
    return pair->first;
}

void* pair_second(Pair* pair) {
    return pair->second;
}

void pair_set(Pair* pair, void* first, void* second) {
    pair_set_first(pair, first);
    pair_set_second(pair, second);
}

void pair_set_first(Pair* pair, void* first) {
    if (pair->first != NULL) {
        pair->t_first->delete_object(pair->first);
    }
    pair->first = first;
}

void pair_set_second(Pair* pair, void* second) {
    if (pair->second != NULL) {
        pair->t_second->delete_object(pair->second);
    }
    pair->second = second;
}

void pair_assign(void** first_ptr, void** second_ptr, Pair* pair) {
    *first_ptr = pair->first;
    *second_ptr = pair->second;
    free(pair);
}

void delete_pair(Pair* pair) {
    if (pair->first != NULL) {
        pair->t_first->delete_object(pair->first);
    }
    if (pair->second != NULL) {
        pair->t_second->delete_object(pair->second);
    }
    free(pair);
}
