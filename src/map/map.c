#include "./map.h"
#include "./cell.h"

#include <stdlib.h>
#include <string.h>

struct _Map {
    HashableType* t_key;
    BaseType* t_value;
    MapCell** container;
    int size;
    int capacity;
};

void update_capacity(Map* map, int new_capacity);
int next_hash(int hash, int capacity);

Map* new_map(HashableType* t_key, BaseType* t_value) {
    Map* map = malloc(sizeof(Map));
    int size = 0, capacity = 1;

    map->t_key = t_key;
    map->t_value = t_value;
    map->container = malloc(capacity * sizeof(MapCell*));
    map->size = size;
    map->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        map->container[i] = NULL;
    }
    return map;
}

void delete_map(Map* map) {
    for (int i = 0; i < map->capacity; i++) {
        MapCell* cell = map->container[i];
        if (cell == NULL) continue;
        delete_mapcell(cell, map->t_key->delete_object, map->t_value->delete_object);
    }
    free(map->container);
    free(map);
}

void* map_get(Map* map, void* key) {
    return map_get_with_default(map, key, NULL);
}

void* map_get_with_default(Map* map, void* key, void* default_value) {
    int hash = map->t_key->hash_object(key) % map->capacity;
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL) {
        if (!cell->deleted && map->t_key->compare_object(cell->key, key) == 0) {
            found = 1;
            break;
        }
        hash = (hash + 1) % map->capacity;
        cell = map->container[hash];
    }

    return found ? cell->value : default_value;
}

void map_add(Map* map, void* key, void* value) {
    int hash = map->t_key->hash_object(key) % map->capacity;
    MapCell* cell = map->container[hash];

    while (cell != NULL && !cell->deleted) {
        if (map->t_key->compare_object(cell->key, key) == 0) break;
        hash = (hash + 1) % map->capacity;
        cell = map->container[hash];
    }

    if (cell != NULL) {
        delete_mapcell(cell, map->t_key->delete_object, map->t_value->delete_object);
    }
    map->container[hash] = new_mapcell(key, value);
    map->size++;

    int std_capacity = 2 * (map->size + 1) - 1;
    if (2 * map->capacity <= std_capacity) update_capacity(map, std_capacity);
}

void map_remove(Map* map, void* key) {
    int hash =  map->t_key->hash_object(key) % map->capacity;
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL) {
        if (!cell->deleted &&  map->t_key->compare_object(cell->key, key) == 0) {
            found = 1;
            break;
        }
        hash = (hash + 1) % map->capacity;
        cell = map->container[hash];
    }

    if (!found) return;

    mapcell_deleted(cell, map->t_key->delete_object, map->t_value->delete_object);
    map->size--;

    int std_capacity = 2 * (map->size + 1) - 1;
    if (2 * std_capacity < map->capacity) update_capacity(map, std_capacity);
}

void update_capacity(Map* map, int new_capacity) {
    MapCell** new_container = malloc(new_capacity * sizeof(MapCell*));
    for (int i = 0; i < new_capacity; i++)
        new_container[i] = NULL;

    for (int i = 0; i < map->capacity; i++) {
        MapCell* cell = map->container[i];
        if (cell == NULL || cell->deleted) {
            if (cell == NULL) continue;
            delete_mapcell(cell, map->t_key->delete_object, map->t_value->delete_object);
            continue;
        }

        int rehash = map->t_key->hash_object(cell->key) % new_capacity;
        while (new_container[rehash] != NULL) {
            rehash = (rehash + 1) % new_capacity;
        }
        new_container[rehash] = cell;
    }

    free(map->container);
    map->container = new_container;
    map->capacity = new_capacity;
}
