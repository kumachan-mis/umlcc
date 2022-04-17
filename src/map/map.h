#ifndef _UMLCC_MAP_H_
#define _UMLCC_MAP_H_

typedef struct _Map Map;

Map*  new_map();
void  delete_map(Map* map, void delete_value(void* item));
void* map_get(Map* map, char* key);
void* map_get_with_default(Map* map, char* key,  void* default_value);
void  map_set(Map* map, char* key, void* value, void delete_value(void* value));
void  map_erase(Map* map, char* key, void delete_value(void* value));

#endif
