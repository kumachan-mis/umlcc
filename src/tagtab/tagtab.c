#include "./tagtab.h"

#include <stdlib.h>

TagTable* new_tagtable(void) {
    TagTable* table = malloc(sizeof(TagTable));
    table->map = new_map(&t_hashable_string, &t_dtype);
    table->outer_scope = NULL;
    return table;
}

TagTable* tagtable_copy(TagTable* table) {
    TagTable* copied_table = malloc(sizeof(TagTable));
    copied_table->map = map_copy(table->map);
    copied_table->outer_scope = NULL;
    if (table->outer_scope != NULL) {
        copied_table->outer_scope = tagtable_copy(table->outer_scope);
    }
    return copied_table;
}

int tagtable_can_define(TagTable* table, char* name) {
    return map_get(table->map, name) == NULL;
}

DType* tagtable_search(TagTable* table, char* name) {
    DType* dtype = NULL;
    while (table != NULL) {
        dtype = map_get(table->map, name);
        if (dtype != NULL) {
            break;
        }
        table = table->outer_scope;
    }
    return dtype;
}

DType* tagtable_define_struct(TagTable* table, char* name, Vector* members) {
    if (!tagtable_can_define(table, name)) {
        return NULL;
    }
    DType* dtype = new_unnamed_struct_dtype(members);
    map_add(table->map, name, dtype);
    return dtype;
}

DType* tagtable_define_enum(TagTable* table, char* name, Vector* members) {
    if (!tagtable_can_define(table, name)) {
        return NULL;
    }
    DType* dtype = new_unnamed_enum_dtype(members);
    map_add(table->map, name, dtype);
    return dtype;
}

TagTable* tagtable_enter_scope(TagTable* table) {
    TagTable* inner_table = new_tagtable();
    inner_table->outer_scope = table;
    return inner_table;
}

TagTable* tagtable_exit_scope(TagTable* table) {
    TagTable* outer_table = table->outer_scope;
    delete_map(table->map);
    free(table);
    return outer_table;
}

void delete_tagtable(TagTable* table) {
    if (table->outer_scope != NULL) {
        delete_tagtable(table->outer_scope);
    }
    delete_map(table->map);
    free(table);
}
