#ifndef UMLCC_MAP_CELL_H
#define UMLCC_MAP_CELL_H

typedef struct MapCell {
    void* key;
    void* value;
    int deleted;
} MapCell;

MapCell* new_mapcell(void* key, void* value);
void delete_mapcell(MapCell* cell, void delete_key(void* key), void delete_value(void* value));
MapCell* mapcell_copy(MapCell* cell, void* copy_key(void* key), void* copy_value(void* value));

#endif
