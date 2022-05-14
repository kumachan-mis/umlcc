#include "./test_map.h"
#include "../src/common/common.h"
#include "../src/map/map.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_map_get_with_default();
void test_map_add();
void test_map_remove();

CU_Suite* add_test_suite_map() {
    CU_Suite* suite = CU_add_suite("test_suite_map", NULL, NULL);
    CU_add_test(suite, "test_map_get_with_default", test_map_get_with_default);
    CU_add_test(suite, "test_map_add", test_map_add);
    CU_add_test(suite, "test_map_remove", test_map_remove);
    return suite;
}

void test_map_get_with_default() {
    Map* map = new_map();
    int* value = map_get(map, "key", str_hash, str_comp);
    CU_ASSERT_PTR_NULL(value);

    int* default_value = new_integer(0);
    value = map_get_with_default(map, "key", default_value, str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 0);

    delete_map(map, delete_str, delete_integer);
}

void test_map_add() {
    Map* map = new_map();
    char* key = NULL;
    int* value = NULL;

    key = new_string("two");
    value = new_integer(2);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    key = new_string("five");
    value = new_integer(5);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    key = new_string("seven");
    value = new_integer(7);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    key = new_string("eight");
    value = new_integer(8);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    value = map_get(map, "five", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 5);

    value = map_get(map, "eight", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 8);

    value = map_get(map, "seven", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "two", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 2);

    value = map_get(map, "ten", str_hash, str_comp);
    CU_ASSERT_EQUAL(value, NULL);

    key = new_string("eight");
    value = new_integer(-1);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    value = map_get(map, "eight", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, -1);

    delete_map(map, delete_str, delete_integer);
}

void test_map_remove() {
    Map* map = new_map();
    char* key = NULL;
    int* value = NULL;

    key = new_string("key");
    value = new_integer(7);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    key = new_string("erased_key");
    value = new_integer(-2);
    map_add(map, key, value, str_hash, str_comp, delete_str, delete_integer);

    value = map_get(map, "key", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "erased_key", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, -2);

    map_remove(map, "erased_key", str_hash, str_comp, delete_str, delete_integer);

    value = map_get(map, "key", str_hash, str_comp);
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "erased_key", str_hash, str_comp);
    CU_ASSERT_PTR_NULL(value);

    delete_map(map, delete_str, delete_integer);
}
