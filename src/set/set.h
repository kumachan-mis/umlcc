#ifndef _UMLCC_SET_H_
#define _UMLCC_SET_H_

typedef struct _Set Set;

Set* new_set();
void delete_set(Set* set);
void set_add(Set* set, int* item);
void set_remove(Set* set, int* item);
int set_contains(Set* set, int* item);
int set_size(Set* set);
Set* set_intersection(Set* set, Set* other);
Set* set_union(Set* set, Set* other);
Set* set_difference(Set* set, Set* other);

#endif
