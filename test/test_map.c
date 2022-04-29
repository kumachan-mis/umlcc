#include "../src/map/map.h"

#include <assert.h>
#include <stdlib.h>

void test_map_set();
void test_map_get_with_default();
void test_map_erase();

int main(void) {
    test_map_set();
    test_map_get_with_default();
    test_map_erase();
}

void test_map_set() {
    Map* map = new_map();
    int* value = NULL;

    value = malloc(sizeof(int));
    *value = 2;
    map_set(map, "two", value, free);

    value = malloc(sizeof(int));
    *value = 5;
    map_set(map, "five", value, free);

    value = malloc(sizeof(int));
    *value = 7;
    map_set(map, "seven", value, free);

    value = malloc(sizeof(int));
    *value = 8;
    map_set(map, "eight", value, free);

    value = map_get(map, "five");
    assert(*value == 5);

    value = map_get(map, "eight");
    assert(*value == 8);

    value = map_get(map, "seven");
    assert(*value == 7);

    value = map_get(map, "two");
    assert(*value == 2);

    value = map_get(map, "ten");
    assert(value == NULL);

    value = malloc(sizeof(int));
    *value = -1;
    map_set(map, "eight", value, free);

    value = map_get(map, "eight");
    assert(*value == -1);

    delete_map(map, free);
}

void test_map_get_with_default() {
    Map* map = new_map();
    int* value = map_get(map, "key");
    assert(value == NULL);

    int* default_value = malloc(sizeof(int));
    *default_value = 0;
    value = map_get_with_default(map, "key", default_value);
    assert(*value == 0);

    delete_map(map, free);
}

void test_map_erase() {
    Map* map = new_map();
    int* value = NULL;

    value = malloc(sizeof(int));
    *value = 7;
    map_set(map, "key", value, free);

    value = malloc(sizeof(int));
    *value = -2;
    map_set(map, "erased_key", value, free);

    value = map_get(map, "key");
    assert(*value == 7);

    value = map_get(map, "erased_key");
    assert(*value == -2);

    map_erase(map, "erased_key", free);

    value = map_get(map, "key");
    assert(*value == 7);

    value = map_get(map, "erased_key");
    assert(value == NULL);
}
