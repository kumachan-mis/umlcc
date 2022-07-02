#include "./test_set.h"
#include "../../src/common/type.h"
#include "../../src/set/set.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_set_copy();
void test_set_add();
void test_set_remove();
void test_set_iter();
void test_set_intersection();
void test_set_intersection_empty();
void test_set_intersection_super();
void test_set_intersection_sub();
void test_set_intersection_foreign();
void test_set_union();
void test_set_union_empty();
void test_set_union_super();
void test_set_union_sub();
void test_set_union_foreign();
void test_set_difference();
void test_set_difference_empty();
void test_set_difference_super();
void test_set_difference_sub();
void test_set_difference_foreign();
void test_set_issubset();
void test_set_issubset_empty();
void test_set_equals();
void test_set_equals_empty();

CU_Suite* add_test_suite_set() {
    CU_Suite* suite = CU_add_suite("test_suite_set", NULL, NULL);
    CU_add_test(suite, "test_set_copy", test_set_copy);
    CU_add_test(suite, "test_set_add", test_set_add);
    CU_add_test(suite, "test_set_remove", test_set_remove);
    CU_add_test(suite, "test_set_iter", test_set_iter);
    CU_add_test(suite, "test_set_intersection", test_set_intersection);
    CU_add_test(suite, "test_set_intersection_empty", test_set_intersection_empty);
    CU_add_test(suite, "test_set_intersection_super", test_set_intersection_super);
    CU_add_test(suite, "test_set_intersection_sub", test_set_intersection_sub);
    CU_add_test(suite, "test_set_intersection_foreign", test_set_intersection_foreign);
    CU_add_test(suite, "test_set_union", test_set_union);
    CU_add_test(suite, "test_set_union_empty", test_set_union_empty);
    CU_add_test(suite, "test_set_union_super", test_set_union_super);
    CU_add_test(suite, "test_set_union_sub", test_set_union_sub);
    CU_add_test(suite, "test_set_union_foreign", test_set_union_foreign);
    CU_add_test(suite, "test_set_difference", test_set_difference);
    CU_add_test(suite, "test_set_difference_empty", test_set_difference_empty);
    CU_add_test(suite, "test_set_difference_super", test_set_difference_super);
    CU_add_test(suite, "test_set_difference_sub", test_set_difference_sub);
    CU_add_test(suite, "test_set_difference_foreign", test_set_difference_foreign);
    CU_add_test(suite, "test_set_issubset", test_set_issubset);
    CU_add_test(suite, "test_set_issubset_empty", test_set_issubset_empty);
    CU_add_test(suite, "test_set_equals", test_set_equals);
    CU_add_test(suite, "test_set_equals_empty", test_set_equals_empty);
    return suite;
}

void test_set_copy() {
    Set* set = new_set(&t_hashable_string);
    char* item = NULL;

    item = new_string("one");
    set_add(set, item);

    item = new_string("two");
    set_add(set, item);

    item = new_string("four");
    set_add(set, item);

    Set* copied_set = set_copy(set);
    delete_set(set);

    CU_ASSERT(set_contains(copied_set, "two"));
    CU_ASSERT(set_contains(copied_set, "one"));
    CU_ASSERT(set_contains(copied_set, "four"));
    CU_ASSERT(!set_contains(copied_set, "fou"));

    delete_set(copied_set);
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
    free(item);

    item = new_integer(2);
    CU_ASSERT(set_contains(set, item));
    free(item);

    item = new_integer(3);
    CU_ASSERT(set_contains(set, item));
    free(item);

    item = new_integer(4);
    CU_ASSERT(!set_contains(set, item));
    free(item);

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
    free(item);

    item = new_integer(0);
    set_remove(set, item);
    free(item);

    item = new_integer(-1);
    CU_ASSERT(set_contains(set, item));
    free(item);

    item = new_integer(1);
    CU_ASSERT(set_contains(set, item));
    free(item);

    item = new_integer(2);
    CU_ASSERT(!set_contains(set, item));
    free(item);

    item = new_integer(3);
    CU_ASSERT(!set_contains(set, item));
    free(item);

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
    free(item);

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

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    Set* intersection_set = set_intersection(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(intersection_set); !set_iter_end(iter, intersection_set);
         iter = set_iter_next(iter, intersection_set)) {
        int* item = set_iter_item(iter, intersection_set);
        CU_ASSERT(*item == 2 || *item == 3);
        count++;
    }
    CU_ASSERT_EQUAL(count, 2);

    delete_set(intersection_set);
    delete_set(other);
    delete_set(set);
}

void test_set_intersection_empty() {
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
    delete_set(other);
    delete_set(set);
}

void test_set_intersection_super() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(0);
    set_add(other, item);

    item = new_integer(1);
    set_add(other, item);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    Set* intersection_set = set_intersection(set, other);
    int count = 0;
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

void test_set_intersection_sub() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    Set* intersection_set = set_intersection(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(intersection_set); !set_iter_end(iter, intersection_set);
         iter = set_iter_next(iter, intersection_set)) {
        int* item = set_iter_item(iter, intersection_set);
        CU_ASSERT(*item == 2 || *item == 3);
        count++;
    }
    CU_ASSERT_EQUAL(count, 2);

    delete_set(intersection_set);
    delete_set(other);
    delete_set(set);
}

void test_set_intersection_foreign() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(4);
    set_add(other, item);

    item = new_integer(-1);
    set_add(other, item);

    item = new_integer(0);
    set_add(other, item);

    Set* intersection_set = set_intersection(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(intersection_set); !set_iter_end(iter, intersection_set);
         iter = set_iter_next(iter, intersection_set)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);

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

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    Set* union_set = set_union(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(union_set); !set_iter_end(iter, union_set);
         iter = set_iter_next(iter, union_set)) {
        int* item = set_iter_item(iter, union_set);
        CU_ASSERT(*item == 1 || *item == 2 || *item == 3 || *item == 4);
        count++;
    }
    CU_ASSERT_EQUAL(count, 4);

    delete_set(union_set);
    delete_set(other);
    delete_set(set);
}

void test_set_union_empty() {
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
    delete_set(other);
    delete_set(set);
}

void test_set_union_super() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(0);
    set_add(other, item);

    item = new_integer(1);
    set_add(other, item);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    Set* union_set = set_union(set, other);
    int count = 0;
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

void test_set_union_sub() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

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
    delete_set(other);
    delete_set(set);
}

void test_set_union_foreign() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(4);
    set_add(other, item);

    item = new_integer(-1);
    set_add(other, item);

    item = new_integer(0);
    set_add(other, item);

    Set* union_set = set_union(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(union_set); !set_iter_end(iter, union_set);
         iter = set_iter_next(iter, union_set)) {
        int* item = set_iter_item(iter, union_set);
        CU_ASSERT(*item == -1 || *item == 0 || *item == 1 || *item == 2 || *item == 3 ||
                  *item == 4);
        count++;
    }
    CU_ASSERT_EQUAL(count, 6);

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

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    Set* difference_set = set_difference(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(difference_set); !set_iter_end(iter, difference_set);
         iter = set_iter_next(iter, difference_set)) {
        int* item = set_iter_item(iter, difference_set);
        CU_ASSERT(*item == 1);
        count++;
    }
    CU_ASSERT_EQUAL(count, 1);

    delete_set(difference_set);
    delete_set(other);
    delete_set(set);
}

void test_set_difference_empty() {
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
    delete_set(other);
    delete_set(set);
}

void test_set_difference_super() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(0);
    set_add(other, item);

    item = new_integer(1);
    set_add(other, item);

    item = new_integer(2);
    set_add(other, item);

    item = new_integer(3);
    set_add(other, item);

    item = new_integer(4);
    set_add(other, item);

    Set* difference_set = set_difference(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(difference_set); !set_iter_end(iter, difference_set);
         iter = set_iter_next(iter, difference_set)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);

    delete_set(difference_set);
    delete_set(other);
    delete_set(set);
}

void test_set_difference_sub() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(3);
    set_add(other, item);

    Set* difference_set = set_difference(set, other);
    int count = 0;
    for (SetIter* iter = set_iter_begin(difference_set); !set_iter_end(iter, difference_set);
         iter = set_iter_next(iter, difference_set)) {
        int* item = set_iter_item(iter, difference_set);
        CU_ASSERT(*item == 1 || *item == 2);
        count++;
    }
    CU_ASSERT_EQUAL(count, 2);

    delete_set(difference_set);
    delete_set(other);
    delete_set(set);
}

void test_set_difference_foreign() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);

    item = new_integer(4);
    set_add(other, item);

    item = new_integer(-1);
    set_add(other, item);

    item = new_integer(0);
    set_add(other, item);

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
    delete_set(other);
    delete_set(set);
}

void test_set_issubset() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(1);
    set_add(set, item);

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(3);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);
    CU_ASSERT(set_issubset(set, other));

    item = new_integer(2);
    set_add(other, item);

    CU_ASSERT(set_issubset(set, other));

    item = new_integer(3);
    set_add(other, item);

    CU_ASSERT(set_issubset(set, other));

    item = new_integer(6);
    set_add(other, item);

    CU_ASSERT(!set_issubset(set, other));

    delete_set(set);
    delete_set(other);
}

void test_set_issubset_empty() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    Set* other = new_set(&t_hashable_integer);
    CU_ASSERT(set_issubset(set, other));

    item = new_integer(0);
    set_add(other, item);

    CU_ASSERT(!set_issubset(set, other));

    delete_set(set);
    delete_set(other);
}

void test_set_equals() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    item = new_integer(2);
    set_add(set, item);

    item = new_integer(4);
    set_add(set, item);

    item = new_integer(5);
    set_add(set, item);

    Set* other = new_set(&t_hashable_integer);
    CU_ASSERT(set_issubset(set, other));

    item = new_integer(2);
    set_add(other, item);

    CU_ASSERT(!set_equals(set, other));

    item = new_integer(5);
    set_add(other, item);

    CU_ASSERT(!set_equals(set, other));

    item = new_integer(4);
    set_add(other, item);

    CU_ASSERT(set_equals(set, other));

    delete_set(set);
    delete_set(other);
}

void test_set_equals_empty() {
    Set* set = new_set(&t_hashable_integer);
    int* item = NULL;

    Set* other = new_set(&t_hashable_integer);
    CU_ASSERT(set_equals(set, other));

    item = new_integer(0);
    set_add(other, item);

    CU_ASSERT(!set_equals(set, other));

    delete_set(set);
    delete_set(other);
}
