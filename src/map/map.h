#ifndef UMLCC_MAP_H
#define UMLCC_MAP_H

#include "../common/type.h"

typedef struct Map Map;
typedef struct MapIter MapIter;

Map* new_map(HashableType* t_key, BaseType* t_value);
Map* map_copy(Map* map);
void* map_get(Map* map, void* key);
void* map_get_with_default(Map* map, void* key, void* default_value);
void map_add(Map* map, void* key, void* value);
void map_remove(Map* map, void* key);
int map_contains(Map* map, void* key);
MapIter* map_iter_begin(Map* map);
MapIter* map_iter_next(MapIter* iter, Map* map);
int map_iter_end(MapIter* iter, Map* map);
void* map_iter_key(MapIter* iter, Map* map);
void* map_iter_value(MapIter* iter, Map* map);
void delete_map(Map* map);

#endif
