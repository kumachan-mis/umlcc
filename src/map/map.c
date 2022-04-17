#include "./map.h"

#include <stdlib.h>
#include <string.h>


typedef struct _MapCell {
    char* key;
    void* value;
    int   deleted;
} MapCell;

struct _Map {
    MapCell** container;
    int       size;
    int       capacity;
};

void     map_update_capacity(Map* map, int new_capacity, void delete_value(void* value));
int      map_calculate_hash(char* key, int capacity);
int      map_calculate_next_hash(int hash, char* key, int capacity);
MapCell* new_mapcell(char* key, void* value);
void     delete_mapcell(MapCell* cell, void delete_value(void* value));
void     mapcell_markas_deleted(MapCell* cell, void delete_value(void* value));


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

void delete_map(Map* map, void delete_value(void* value)) {
    for (int i = 0; i < map->capacity; i++) {
        MapCell* cell = map->container[i];
        if (cell != NULL) delete_mapcell(cell, delete_value);
    }
    free(map->container);
    free(map);
}

void* map_get(Map* map, char* key) {
    return map_get_with_default(map, key, NULL);
}

void* map_get_with_default(Map* map, char* key,  void* default_value) {
    int hash = map_calculate_hash(key, map->capacity);
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL){
        if (!cell->deleted && strcmp(cell->key, key) == 0) {
           found = 1;
           break;
        }
        hash = map_calculate_next_hash(hash, key, map->capacity);
        cell = map->container[hash];
    }

    return found? cell->value : default_value;
}

void map_set(Map* map, char* key, void* value, void delete_value(void* value)) {
    int hash = map_calculate_hash(key, map->capacity);
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL && !cell->deleted){
        if (strcmp(cell->key, key) == 0) {
            found = 1;
            break;
        }
        hash = map_calculate_next_hash(hash, key, map->capacity);
        cell = map->container[hash];
    }

    if (found) {
        delete_value(cell->value);
        cell->value = value;
        return;
    }

    if (cell != NULL) delete_mapcell(cell, delete_value);
    map->container[hash] = new_mapcell(key, value);
    map->size++;

    int std_capacity = 2 * (map->size + 1) - 1;
    if (2 * map->capacity <= std_capacity) {
        map_update_capacity(map, std_capacity, delete_value);
    }
}

void map_erase(Map* map, char* key, void delete_value(void* value)) {
    int hash = map_calculate_hash(key, map->capacity);
    MapCell* cell = map->container[hash];

    int found = 0;
    while (cell != NULL){
        if (!cell->deleted && strcmp(cell->key, key) == 0) {
           found = 1;
           break;
        }
        hash = map_calculate_next_hash(hash, key, map->capacity);
        cell = map->container[hash];
    }

    if (!found) return;

    mapcell_markas_deleted(cell, delete_value);
    map->size--;

    int std_capacity = 2 * (map->size + 1) - 1;
    if (2 * std_capacity < map->capacity) {
        map_update_capacity(map, std_capacity, delete_value);
    }
}

void map_update_capacity(Map* map, int new_capacity, void delete_value(void* value)) {
    MapCell** new_container = malloc(new_capacity * sizeof(MapCell*));
    for (int i = 0; i < new_capacity; i++) new_container[i] = NULL;

    for (int i = 0; i < map->capacity; i++) {
        MapCell* cell = map->container[i];
        if (cell == NULL || cell->deleted) {
            if (cell != NULL) delete_mapcell(cell, delete_value);
            continue;
        }

        int rehash = map_calculate_hash(cell->key, new_capacity);
        while (new_container[rehash] != NULL) {
            rehash = map_calculate_next_hash(rehash, cell->key, new_capacity);
        }
        new_container[rehash] = cell;
    }

    free(map->container);
    map->container = new_container;
    map->capacity = new_capacity;
}

int map_calculate_hash(char* key, int capacity) {
    int a = 151, b = 587;
    int hash = 0;

    char* key_ptr = key;
    while (*key_ptr != '\0') {
        hash = (*key_ptr * a + hash) % b;
        key_ptr++;
    }

    return hash % capacity;
}

int map_calculate_next_hash(int hash, char* key, int capacity) {
    return (hash + 1) % capacity;
}

MapCell* new_mapcell(char* key, void* value) {
    MapCell* cell = malloc(sizeof(MapCell));
    cell->key =  malloc((strlen(key) + 1) * sizeof(char));
    strcpy(cell->key, key);
    cell->value = value;
    cell->deleted = 0;
    return cell;
}

void delete_mapcell(MapCell* cell, void delete_value(void* value)) {
    if (cell->key != NULL) {
        free(cell->key);
    }
    if (cell->value != NULL) {
        delete_value(cell->value);
    }
    free(cell);
}

void mapcell_markas_deleted(MapCell* cell, void delete_value(void* value)) {
    if (cell->key != NULL) {
        free(cell->key);
        cell->key = NULL;
    }
    if (cell->value != NULL) {
        delete_value(cell->value);
        cell->value = NULL;
    }
    cell->deleted = 1;
}
