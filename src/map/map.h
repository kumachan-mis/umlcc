#ifndef _UMLCC_MAP_H_
#define _UMLCC_MAP_H_

#include "../common/common.h"

typedef struct _Map Map;
typedef struct _MapIter MapIter;

Map* new_map(HashableType* t_key, BaseType* t_value);
void delete_map(Map* map);
void* map_get(Map* map, void* key);
void* map_get_with_default(Map* map, void* key, void* default_value);
void map_add(Map* map, void* key, void* value);
void map_remove(Map* map, void* key);
MapIter* map_iter_begin(Map* map);
MapIter* map_iter_next(MapIter* iter, Map* map);
int map_iter_end(MapIter* iter, Map* map);
void* map_iter_key(MapIter* iter, Map* map);
void* map_iter_value(MapIter* iter, Map* map);

#endif
