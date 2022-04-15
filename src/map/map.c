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
    int       capacity;
};

int      map_calculate_hash(Map* map, char* key);
int      map_calculate_next_hash(Map* map, char* key, int hash);
MapCell* new_mapcell(char* key, void* value);
void     delete_mapcell(MapCell* cell, void delete_value(void* value));
void     mapcell_markas_deleted(MapCell* cell, void delete_value(void* value));


Map* new_map() {
    Map* map = malloc(sizeof(Map));
    int capacity = 1;

    map->container = malloc(capacity * sizeof(MapCell*));
    map->capacity = capacity;
    for (int i = 0; i < capacity; i++) {
        map->container[i] = NULL;
    }
    return map;
}

void delete_map(Map* map, void delete_value(void* value)) {
    for (int i = 0; i < map->capacity; i++) {
        delete_mapcell(map->container[i], delete_value);
    }
    free(map->container);
    free(map);
}

void* map_get(Map* map, char* key) {
    return map_get_with_default(map, key, NULL);
}

void* map_get_with_default(Map* map, char* key,  void* default_value) {
    int hash = map_calculate_hash(map, key);
    MapCell* cell = map->container[hash];
    while (cell != NULL){
        if (!cell->deleted && strcmp(cell->key, key) == 0) {
            return cell->value;
        }
        hash = map_calculate_next_hash(map, key, hash);
        cell = map->container[hash];
    }

    return default_value;
}

void map_set(Map* map, char* key, void* value, void delete_value(void* value)) {
    int hash = map_calculate_hash(map, key);
    MapCell* cell = map->container[hash];
    while (cell != NULL && !cell->deleted){
        if (strcmp(cell->key, key) == 0) {
            delete_value(cell->value);
            cell->value = value;
            return;
        }
        hash = map_calculate_next_hash(map, key, hash);
        cell = map->container[hash];
    }

    if (cell != NULL) delete_mapcell(cell, delete_value);
    map->container[hash] = new_mapcell(key, value);

    int new_capacity = 2 * (map->capacity + 1) - 1;
    map->container = realloc(map->container, new_capacity * sizeof(MapCell*));
    for (int i = map->capacity + 1; i < new_capacity; i++) {
        map->container[i] = NULL;
    }
    map->capacity = new_capacity;
}

void map_erase(Map* map, char* key, void delete_value(void* value)) {
    int hash = map_calculate_hash(map, key);
    MapCell* cell = map->container[hash];
    while (cell != NULL){
        if (!cell->deleted && strcmp(cell->key, key) == 0) {
            mapcell_markas_deleted(cell, delete_value);
        }
        hash = map_calculate_next_hash(map, key, hash);
        cell = map->container[hash];
    }
}

int map_calculate_hash(Map* map, char* key) {
    int a = 154303, b = 628811;
    int hash = 0;

    char* key_ptr = key;
    while (*key_ptr != '\0') {
        hash = (*key_ptr * a + hash) % b;
    }

    return hash % map->capacity;
}

int map_calculate_next_hash(Map* map, char* key, int hash) {
    return (hash + 1) % map->capacity;
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
