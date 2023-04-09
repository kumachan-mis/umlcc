#ifndef UMLCC_DTYPE_TAGTAB_H
#define UMLCC_DTYPE_TAGTAB_H

#include "../dtype/dtype.h"
#include "../map/map.h"

typedef struct TagTable TagTable;

struct TagTable {
    Map* map;
    TagTable* outer_scope;
};

TagTable* new_tagtable(void);
TagTable* tagtable_copy(TagTable* table);
int tagtable_can_define(TagTable* table, char* name);
DType* tagtable_define_struct(TagTable* table, char* name, Vector* members);
DType* tagtable_define_enum(TagTable* table, char* name, Vector* members);
DType* tagtable_search(TagTable* table, char* name);
TagTable* tagtable_enter_scope(TagTable* table);
TagTable* tagtable_exit_scope(TagTable* table);
void delete_tagtable(TagTable* table);

#endif
