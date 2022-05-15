#ifndef _UMLCC_MAP_CELL_H_
#define _UMLCC_MAP_CELL_H_

typedef struct _MapCell {
    void* key;
    void* value;
    int deleted;
} MapCell;

MapCell* new_mapcell(void* key, void* value);
void delete_mapcell(MapCell* cell, void delete_key(void* key), void delete_value(void* value));
MapCell* mapcell_copy(MapCell* cell, void* copy_key(void* key), void* copy_value(void* value));
void mapcell_deleted(MapCell* cell, void delete_key(void* key), void delete_value(void* value));

#endif
