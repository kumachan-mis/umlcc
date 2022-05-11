#ifndef _UMLCC_REGALLOC_LIFETIME_H_
#define _UMLCC_REGALLOC_LIFETIME_H_

typedef struct _Lifetime {
    int firstdef_index;
    int lastuse_index;
} Lifetime;

Lifetime* new_lifetime(int firstdef_index, int lastuse_index);
void delete_lifetime(Lifetime* lifetime);

#endif
