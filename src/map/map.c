#include "./map.h"
#include "./cell.h"

#include <stdlib.h>
#include <string.h>

struct _Map {
    MapCell** container;
    int size;
    int capacity;
};

void update_capacity(Map* map, int new_capacity, int hash_key(void* key, int max),
                     void delete_key(void* key), void delete_value(void* value));
int next_hash(int hash, int capacity);

Map* new_map() {
    Map* map = malloc(sizeof(Map));
    int size = 0, capacity = 1;

    map->container = malloc(capacity * sizeof(MapCell*));
    map->size = size;
    map->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        map->container[i] = NULL;
    }
    return map;
}

void delete_map(Map* map, void delete_key(void* key), void delete_value(void* value)) {
    for (int i = 0; i < map->capacity; i++) {
        MapCell* cell = map->container[i];
        if (cell != NULL) delete_mapcell(cell, delete_key, delete_value);
    }
    free(map->container);
    free(map);
}

void* map_get(Map* map, void* key, int hash_key(void* key, int max),
              int comp_key(void* a, void* b)) {
    return map_get_with_default(map, key, NULL, hash_key, comp_key);
}

void* map_get_with_default(Map* map, void* key, void* default_value,
                           int hash_key(void* key, int max), int comp_key(void* a, void* b)) {
    int hash = hash_key(key, map->capacity);
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL) {
        if (!cell->deleted && comp_key(cell->key, key) == 0) {
            found = 1;
            break;
        }
        hash = next_hash(hash, map->capacity);
        cell = map->container[hash];
    }

    return found ? cell->value : default_value;
}

void map_add(Map* map, void* key, void* value, int hash_key(void* key, int max),
             int comp_key(void* a, void* b), void delete_key(void* key),
             void delete_value(void* value)) {
    int hash = hash_key(key, map->capacity);
    MapCell* cell = map->container[hash];

    while (cell != NULL && !cell->deleted) {
        if (comp_key(cell->key, key) == 0) break;
        hash = next_hash(hash, map->capacity);
        cell = map->container[hash];
    }

    if (cell != NULL) delete_mapcell(cell, delete_key, delete_value);
    map->container[hash] = new_mapcell(key, value);
    map->size++;

    int std_capacity = 2 * (map->size + 1) - 1;
    if (2 * map->capacity <= std_capacity)
        update_capacity(map, std_capacity, hash_key, delete_key, delete_value);
}

void map_remove(Map* map, void* key, int hash_key(void* key, int max),
                int comp_key(void* a, void* b), void delete_key(void* key),
                void delete_value(void* value)) {
    int hash = hash_key(key, map->capacity);
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL) {
        if (!cell->deleted && comp_key(cell->key, key) == 0) {
            found = 1;
            break;
        }
        hash = next_hash(hash, map->capacity);
        cell = map->container[hash];
    }

    if (!found) return;

    mapcell_markas_deleted(cell, delete_key, delete_value);
    map->size--;

    int std_capacity = 2 * (map->size + 1) - 1;
    if (2 * std_capacity < map->capacity)
        update_capacity(map, std_capacity, hash_key, delete_key, delete_value);
}

void update_capacity(Map* map, int new_capacity, int hash_key(void* key, int max),
                     void delete_key(void* key), void delete_value(void* value)) {
    MapCell** new_container = malloc(new_capacity * sizeof(MapCell*));
    for (int i = 0; i < new_capacity; i++)
        new_container[i] = NULL;

    for (int i = 0; i < map->capacity; i++) {
        MapCell* cell = map->container[i];
        if (cell == NULL || cell->deleted) {
            if (cell != NULL) delete_mapcell(cell, delete_key, delete_value);
            continue;
        }

        int rehash = hash_key(cell->key, new_capacity);
        while (new_container[rehash] != NULL) {
            rehash = next_hash(rehash, new_capacity);
        }
        new_container[rehash] = cell;
    }

    free(map->container);
    map->container = new_container;
    map->capacity = new_capacity;
}

int next_hash(int hash, int max) {
    return (hash + 1) % max;
}
