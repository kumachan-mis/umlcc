#ifndef UMLCC_PAIR_H
#define UMLCC_PAIR_H

#include "../common/type.h"

typedef struct Pair Pair;

extern BaseType t_pair;

Pair* new_pair(BaseType* t_first, BaseType* t_second);
Pair* pair_copy(Pair* pair);
void* pair_first(Pair* pair);
void* pair_second(Pair* pair);
void pair_set(Pair* pair, void* first, void* second);
void pair_set_first(Pair* pair, void* first);
void pair_set_second(Pair* pair, void* second);
void pair_assign(void** first_ptr, void** second_ptr, Pair* pair);
void delete_pair(Pair* pair);

#endif
