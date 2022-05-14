#ifndef _UMLCC_SET_H_
#define _UMLCC_SET_H_

#include "../common/common.h"

typedef struct _Set Set;

Set* new_set(HashableType* t_item);
void delete_set(Set* set);
void set_add(Set* set, void* item);
void set_remove(Set* set, void* item);
int set_contains(Set* set, void* item);
Set* set_intersection(Set* set, Set* other);
Set* set_union(Set* set, Set* other);
Set* set_difference(Set* set, Set* other);

#endif
