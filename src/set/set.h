#ifndef _UMLCC_SET_H_
#define _UMLCC_SET_H_

typedef struct _Set Set;

Set* new_set();
void delete_set(Set* set, void delete_item(void* item));

void set_add(Set* set, void* item, int hash_item(void* item, int max),
             int comp_item(void* a, void* b), void delete_item(void* item));

void set_remove(Set* set, void* item, int hash_item(void* item, int max),
                int comp_item(void* a, void* b), void delete_item(void* item));

int set_contains(Set* set, void* item, int hash_item(void* item, int max),
                 int comp_item(void* a, void* b));

Set* set_intersection(Set* set, Set* other, int hash_item(void* item, int max),
                      int comp_item(void* a, void* b), void delete_item(void* item));

Set* set_union(Set* set, Set* other, int hash_item(void* item, int max),
               int comp_item(void* a, void* b), void delete_item(void* item));

Set* set_difference(Set* set, Set* other, int hash_item(void* item, int max),
                    int comp_item(void* a, void* b), void delete_item(void* item));

#endif
