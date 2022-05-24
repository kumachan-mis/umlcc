#ifndef UMLCC_SET_H
#define UMLCC_SET_H

#include "../common/common.h"

typedef struct Set Set;
typedef struct SetIter SetIter;

Set* new_set(HashableType* t_item);
Set* set_copy(Set* set);
void set_add(Set* set, void* item);
void set_remove(Set* set, void* item);
int set_contains(Set* set, void* item);
SetIter* set_iter_begin(Set* set);
SetIter* set_iter_next(SetIter* iter, Set* set);
int set_iter_end(SetIter* iter, Set* set);
void* set_iter_item(SetIter* iter, Set* set);
Set* set_intersection(Set* set, Set* other);
Set* set_union(Set* set, Set* other);
Set* set_difference(Set* set, Set* other);
int set_issubset(Set* set, Set* other);
int set_equals(Set* set, Set* other);
void delete_set(Set* set);

#endif
