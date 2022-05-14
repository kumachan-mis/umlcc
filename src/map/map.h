#ifndef _UMLCC_MAP_H_
#define _UMLCC_MAP_H_

typedef struct _Map Map;

Map* new_map();
void delete_map(Map* map, void delete_key(void* key), void delete_value(void* value));

void* map_get(Map* map, void* key, int hash_key(void* key, int max),
              int comp_key(void* a, void* b));

void* map_get_with_default(Map* map, void* key, void* default_value,
                           int hash_key(void* key, int max), int comp_key(void* a, void* b));

void map_set(Map* map, void* key, void* value, int hash_key(void* key, int max),
             int comp_key(void* a, void* b), void delete_key(void* key),
             void delete_value(void* value));

void map_erase(Map* map, void* key, int hash_key(void* key, int max),
               int comp_key(void* a, void* b), void delete_key(void* key),
               void delete_value(void* value));

#endif
