#include "./test_set.h"
#include "../src/common/common.h"
#include "../src/set/set.h"

#include <CUnit/Basic.h>

void test_set_add();
void test_set_remove();
void test_set_iter();
void test_set_intersection();
void test_set_union();
void test_set_difference();

CU_Suite* add_test_suite_set() {
    CU_Suite* suite = CU_add_suite("test_suite_set", NULL, NULL);
    CU_add_test(suite, "test_set_add", test_set_add);
    CU_add_test(suite, "test_set_remove", test_set_remove);
    CU_add_test(suite, "test_set_iter", test_set_iter);
    CU_add_test(suite, "test_set_intersection", test_set_intersection);
    CU_add_test(suite, "test_set_union", test_set_union);
    CU_add_test(suite, "test_set_difference", test_set_difference);
    return suite;
}

void test_set_add() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    CU_ASSERT(set_contains(set, item));

    item = new_integer(1);
    CU_ASSERT(set_contains(set, item));
    t_hashable_integer.delete_object(item);

    item = new_integer(2);
    CU_ASSERT(set_contains(set, item));
    t_hashable_integer.delete_object(item);

    item = new_integer(3);
    CU_ASSERT(set_contains(set, item));
    t_hashable_integer.delete_object(item);

    item = new_integer(4);
    CU_ASSERT(!set_contains(set, item));
    t_hashable_integer.delete_object(item);

    delete_set(set);
}

void test_set_remove() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(-1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(2);
    set_remove(set, item);
    t_hashable_integer.delete_object(item);

    item = new_integer(0);
    set_remove(set, item);
    t_hashable_integer.delete_object(item);

    item = new_integer(-1);
    CU_ASSERT(set_contains(set, item));
    t_hashable_integer.delete_object(item);

    item = new_integer(1);
    CU_ASSERT(set_contains(set, item));
    t_hashable_integer.delete_object(item);

    item = new_integer(2);
    CU_ASSERT(!set_contains(set, item));
    t_hashable_integer.delete_object(item);

    item = new_integer(3);
    CU_ASSERT(!set_contains(set, item));
    t_hashable_integer.delete_object(item);

    delete_set(set);
}

void test_set_iter() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(-1);
    set_add(set, item);

    item = new_integer(0);
    set_add(set, item);

    int count = 0;
    for (SetIter* iter = set_iter_begin(set); !set_iter_end(iter, set);
         iter = set_iter_next(iter, set)) {
        int* item = set_iter_item(iter, set);
        CU_ASSERT(*item == -1 || *item == 0 || *item == 1);
        count++;
    }
    CU_ASSERT_EQUAL(count, 3);

    item = new_integer(0);
    set_remove(set, item);
    t_hashable_integer.delete_object(item);

    count = 0;
    for (SetIter* iter = set_iter_begin(set); !set_iter_end(iter, set);
         iter = set_iter_next(iter, set)) {
        int* item = set_iter_item(iter, set);
        CU_ASSERT(*item == -1 || *item == 1);
        count++;
    }
    CU_ASSERT_EQUAL(count, 2);

    delete_set(set);
}

void test_set_intersection() {}

void test_set_union() {}

void test_set_difference() {}
