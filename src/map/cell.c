#include "./cell.h"

#include <stdlib.h>

MapCell* new_mapcell(void* key, void* value) {
    MapCell* cell = malloc(sizeof(MapCell));
    cell->key = key;
    cell->value = value;
    cell->deleted = 0;
    return cell;
}

void delete_mapcell(MapCell* cell, void delete_key(void* key), void delete_value(void* value)) {
    if (cell->key != NULL) delete_key(cell->key);
    if (cell->value != NULL) delete_value(cell->value);
    free(cell);
}

void mapcell_markas_deleted(MapCell* cell, void delete_key(void* key),
                            void delete_value(void* value)) {
    if (cell->key != NULL) {
        delete_key(cell->key);
        cell->key = NULL;
    }
    if (cell->value != NULL) {
        delete_value(cell->value);
        cell->value = NULL;
    }
    cell->deleted = 1;
}
