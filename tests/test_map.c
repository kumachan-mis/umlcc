#include "./test_map.h"
#include "../src/common/common.h"
#include "../src/map/map.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_map_set();
void test_map_get_with_default();
void test_map_erase();

CU_Suite* add_test_suite_map() {
    CU_Suite* suite = CU_add_suite("test_suite_map", NULL, NULL);
    CU_add_test(suite, "test_map_set", test_map_set);
    CU_add_test(suite, "test_map_get_with_default", test_map_get_with_default);
    CU_add_test(suite, "test_map_erase", test_map_erase);
    return suite;
}

void test_map_set() {
    Map* map = new_map();
    int* value = NULL;

    value = new_integer(2);
    map_set(map, "two", value, free);

    value = new_integer(5);
    map_set(map, "five", value, free);

    value = new_integer(7);
    map_set(map, "seven", value, free);

    value = new_integer(8);
    map_set(map, "eight", value, free);

    value = map_get(map, "five");
    CU_ASSERT_EQUAL(*value, 5);

    value = map_get(map, "eight");
    CU_ASSERT_EQUAL(*value, 8);

    value = map_get(map, "seven");
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "two");
    CU_ASSERT_EQUAL(*value, 2);

    value = map_get(map, "ten");
    CU_ASSERT_EQUAL(value, NULL);

    value = new_integer(-1);
    map_set(map, "eight", value, free);

    value = map_get(map, "eight");
    CU_ASSERT_EQUAL(*value, -1);

    delete_map(map, free);
}

void test_map_get_with_default() {
    Map* map = new_map();
    int* value = map_get(map, "key");
    CU_ASSERT_PTR_NULL(value);

    int* default_value = new_integer(0);
    value = map_get_with_default(map, "key", default_value);
    CU_ASSERT_EQUAL(*value, 0);

    delete_map(map, free);
}

void test_map_erase() {
    Map* map = new_map();
    int* value = NULL;

    value = new_integer(7);
    map_set(map, "key", value, free);

    value = new_integer(-2);
    map_set(map, "erased_key", value, free);

    value = map_get(map, "key");
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "erased_key");
    CU_ASSERT_EQUAL(*value, -2);

    map_erase(map, "erased_key", free);

    value = map_get(map, "key");
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "erased_key");
    CU_ASSERT_PTR_NULL(value);
}
