#include "./test_map.h"
#include "../src/common/common.h"
#include "../src/map/map.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_map_get_with_default();
void test_map_add();
void test_map_remove();
void test_map_iter();

CU_Suite* add_test_suite_map() {
    CU_Suite* suite = CU_add_suite("test_suite_map", NULL, NULL);
    CU_add_test(suite, "test_map_get_with_default", test_map_get_with_default);
    CU_add_test(suite, "test_map_add", test_map_add);
    CU_add_test(suite, "test_map_remove", test_map_remove);
    CU_add_test(suite, "test_map_iter", test_map_iter);
    return suite;
}

void test_map_get_with_default() {
    Map* map = new_map(&t_hashable_string, &t_integer);
    int* value = map_get(map, "key");
    CU_ASSERT_PTR_NULL(value);

    int* default_value = new_integer(0);
    value = map_get_with_default(map, "key", default_value);
    CU_ASSERT_EQUAL(*value, 0);

    free(default_value);
    delete_map(map);
}

void test_map_add() {
    Map* map = new_map(&t_hashable_string, &t_integer);
    char* key = NULL;
    int* value = NULL;

    key = new_string("two");
    value = new_integer(2);
    map_add(map, key, value);

    key = new_string("five");
    value = new_integer(5);
    map_add(map, key, value);

    key = new_string("seven");
    value = new_integer(7);
    map_add(map, key, value);

    key = new_string("eight");
    value = new_integer(8);
    map_add(map, key, value);

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

    key = new_string("eight");
    value = new_integer(-1);
    map_add(map, key, value);

    value = map_get(map, "eight");
    CU_ASSERT_EQUAL(*value, -1);

    delete_map(map);
}

void test_map_remove() {
    Map* map = new_map(&t_hashable_string, &t_integer);
    char* key = NULL;
    int* value = NULL;

    key = new_string("key");
    value = new_integer(7);
    map_add(map, key, value);

    key = new_string("erased_key");
    value = new_integer(-2);
    map_add(map, key, value);

    value = map_get(map, "key");
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "erased_key");
    CU_ASSERT_EQUAL(*value, -2);

    map_remove(map, "erased_key");

    value = map_get(map, "key");
    CU_ASSERT_EQUAL(*value, 7);

    value = map_get(map, "erased_key");
    CU_ASSERT_PTR_NULL(value);

    delete_map(map);
}

void test_map_iter() {
    Map* map = new_map(&t_hashable_string, &t_integer);
    char* key = NULL;
    int* value = NULL;

    key = new_string("two");
    value = new_integer(2);
    map_add(map, key, value);

    key = new_string("five");
    value = new_integer(5);
    map_add(map, key, value);

    key = new_string("seven");
    value = new_integer(7);
    map_add(map, key, value);

    key = new_string("negative one");
    value = new_integer(-1);
    map_add(map, key, value);

    int count = 0;
    for (MapIter* iter = map_iter_begin(map); !map_iter_end(iter, map);
         iter = map_iter_next(iter, map)) {
        value = map_iter_value(iter, map);
        CU_ASSERT(*value == 2 || *value == 5 || *value == 7 || *value == -1);
        count++;
    }
    CU_ASSERT_EQUAL(count, 4);

    map_remove(map, "five");

    count = 0;
    for (MapIter* iter = map_iter_begin(map); !map_iter_end(iter, map);
         iter = map_iter_next(iter, map)) {
        value = map_iter_value(iter, map);
        CU_ASSERT(*value == 2 || *value == 7 || *value == -1);
        count++;
    }
    CU_ASSERT_EQUAL(count, 3);

    delete_map(map);
}