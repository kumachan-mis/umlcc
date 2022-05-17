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
    delete_key(cell->key);
    delete_value(cell->value);
    free(cell);
}

MapCell* mapcell_copy(MapCell* cell, void* copy_key(void* key), void* copy_value(void* value)) {
    MapCell* copied_cell = malloc(sizeof(MapCell));
    copied_cell->key = copy_key(cell->key);
    copied_cell->value = copy_value(cell->value);
    copied_cell->deleted = cell->deleted;
    return copied_cell;
}
