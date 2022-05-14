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

void test_set_intersection() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);
    Set* intersection_set = set_intersection(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(intersection_set); !set_iter_end(iter, intersection_set);
         iter = set_iter_next(iter, intersection_set)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);
    delete_set(intersection_set);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    intersection_set = set_intersection(set, other);
    count = 0;
    for (SetIter* iter = set_iter_begin(intersection_set); !set_iter_end(iter, intersection_set);
         iter = set_iter_next(iter, intersection_set)) {
        int* item = set_iter_item(iter, intersection_set);
        CU_ASSERT(*item == 2 || *item == 3);
        count++;
    }
    CU_ASSERT_EQUAL(count, 2);
    delete_set(intersection_set);

    item = new_integer(1);
    set_add(other, item);

    item = new_integer(0);
    set_add(other, item);

    intersection_set = set_intersection(set, other);
    count = 0;
    for (SetIter* iter = set_iter_begin(intersection_set); !set_iter_end(iter, intersection_set);
         iter = set_iter_next(iter, intersection_set)) {
        int* item = set_iter_item(iter, intersection_set);
        CU_ASSERT(*item == 1 || *item == 2 || *item == 3);
        count++;
    }
    CU_ASSERT_EQUAL(count, 3);
    delete_set(intersection_set);

    delete_set(other);
    delete_set(set);
}

void test_set_union() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);
    Set* union_set = set_union(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(union_set); !set_iter_end(iter, union_set);
         iter = set_iter_next(iter, union_set)) {
        int* item = set_iter_item(iter, union_set);
        CU_ASSERT(*item == 1 || *item == 2 || *item == 3);
        count++;
    }
    CU_ASSERT_EQUAL(count, 3);
    delete_set(union_set);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    union_set = set_union(set, other);
    count = 0;
    for (SetIter* iter = set_iter_begin(union_set); !set_iter_end(iter, union_set);
         iter = set_iter_next(iter, union_set)) {
        int* item = set_iter_item(iter, union_set);
        CU_ASSERT(*item == 1 || *item == 2 || *item == 3 || *item == 4);
        count++;
    }
    CU_ASSERT_EQUAL(count, 4);
    delete_set(union_set);

    item = new_integer(1);
    set_add(other, item);

    item = new_integer(0);
    set_add(other, item);

    union_set = set_union(set, other);
    count = 0;
    for (SetIter* iter = set_iter_begin(union_set); !set_iter_end(iter, union_set);
         iter = set_iter_next(iter, union_set)) {
        int* item = set_iter_item(iter, union_set);
        CU_ASSERT(*item == 0 || *item == 1 || *item == 2 || *item == 3 || *item == 4);
        count++;
    }
    CU_ASSERT_EQUAL(count, 5);
    delete_set(union_set);

    delete_set(other);
    delete_set(set);
}

void test_set_difference() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);
    Set* difference_set = set_difference(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(difference_set); !set_iter_end(iter, difference_set);
         iter = set_iter_next(iter, difference_set)) {
        int* item = set_iter_item(iter, difference_set);
        CU_ASSERT(*item == 1 || *item == 2 || *item == 3);
        count++;
    }
    CU_ASSERT_EQUAL(count, 3);
    delete_set(difference_set);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    difference_set = set_difference(set, other);
    count = 0;
    for (SetIter* iter = set_iter_begin(difference_set); !set_iter_end(iter, difference_set);
         iter = set_iter_next(iter, difference_set)) {
        int* item = set_iter_item(iter, difference_set);
        CU_ASSERT(*item == 1);
        count++;
    }
    CU_ASSERT_EQUAL(count, 1);
    delete_set(difference_set);

    item = new_integer(1);
    set_add(other, item);

    item = new_integer(0);
    set_add(other, item);

    difference_set = set_difference(set, other);
    count = 0;
    for (SetIter* iter = set_iter_begin(difference_set); !set_iter_end(iter, difference_set);
         iter = set_iter_next(iter, difference_set)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);
    delete_set(difference_set);

    delete_set(other);
    delete_set(set);
}
