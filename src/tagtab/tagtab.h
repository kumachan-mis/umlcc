#ifndef UMLCC_DTYPE_TAGTAB_H
#define UMLCC_DTYPE_TAGTAB_H

#include "../dtype/dtype.h"
#include "../map/map.h"

typedef struct TagTable TagTable;

struct TagTable {
    Map* struct_map;
    TagTable* outer_scope;
};

TagTable* new_tagtable(void);
TagTable* tagtable_copy(TagTable* table);
int tagtable_can_define_struct(TagTable* table, char* name);
DType* tagtable_define_struct(TagTable* table, char* name, Vector* members);
DType* tagtable_search_struct(TagTable* table, char* name);
TagTable* tagtable_enter_scope(TagTable* table);
TagTable* tagtable_exit_scope(TagTable* table);
void delete_tagtable(TagTable* table);

#endif