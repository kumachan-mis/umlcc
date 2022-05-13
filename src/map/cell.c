#include "./cell.h"
#include "../common/common.h"

#include <stdlib.h>

MapCell* new_mapcell(char* key, void* value) {
    MapCell* cell = malloc(sizeof(MapCell));
    cell->key = string_copy(key);
    cell->value = value;
    cell->deleted = 0;
    return cell;
}

void delete_mapcell(MapCell* cell, void delete_value(void* value)) {
    if (cell->key != NULL) free(cell->key);
    if (cell->value != NULL) delete_value(cell->value);
    free(cell);
}

void mapcell_markas_deleted(MapCell* cell, void delete_value(void* value)) {
    if (cell->key != NULL) {
        free(cell->key);
        cell->key = NULL;
    }
    if (cell->value != NULL) {
        delete_value(cell->value);
        cell->value = NULL;
    }
    cell->deleted = 1;
}
