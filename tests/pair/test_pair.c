#include "./test_pair.h"
#include "../../src/common/type.h"
#include "../../src/pair/pair.h"

#include <stdlib.h>

void test_pair_copy(void);
void test_pair_set(void);
void test_pair_set_first(void);
void test_pair_set_second(void);
void test_pair_assign(void);
void test_pair_assign_first_null(void);
void test_pair_assign_second_null(void);

CU_Suite* add_test_suite_pair(void) {
    CU_Suite* suite = CU_add_suite("test_suite_pair", NULL, NULL);
    CU_ADD_TEST(suite, test_pair_copy);
    CU_ADD_TEST(suite, test_pair_set);
    CU_ADD_TEST(suite, test_pair_set_first);
    CU_ADD_TEST(suite, test_pair_set_second);
    CU_ADD_TEST(suite, test_pair_assign);
    CU_ADD_TEST(suite, test_pair_assign_first_null);
    CU_ADD_TEST(suite, test_pair_assign_second_null);
    return suite;
}

void test_pair_copy(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    first = pair_first(pair);
    CU_ASSERT_PTR_NULL(first);

    second = pair_second(pair);
    CU_ASSERT_PTR_NULL(second);

    first = new_integer(1);
    second = new_string("one");
    pair_set(pair, first, second);

    Pair* copied_pair = pair_copy(pair);
    delete_pair(pair);

    first = pair_first(copied_pair);
    CU_ASSERT_EQUAL(*first, 1);

    second = pair_second(copied_pair);
    CU_ASSERT_STRING_EQUAL(second, "one");

    delete_pair(copied_pair);
}

void test_pair_set(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    first = pair_first(pair);
    CU_ASSERT_PTR_NULL(first);

    second = pair_second(pair);
    CU_ASSERT_PTR_NULL(second);

    first = new_integer(1);
    second = new_string("one");
    pair_set(pair, first, second);

    first = pair_first(pair);
    CU_ASSERT_EQUAL(*first, 1);

    second = pair_second(pair);
    CU_ASSERT_STRING_EQUAL(second, "one");

    first = new_integer(2);
    second = new_string("two");
    pair_set(pair, first, second);

    first = pair_first(pair);
    CU_ASSERT_EQUAL(*first, 2);

    second = pair_second(pair);
    CU_ASSERT_STRING_EQUAL(second, "two");

    delete_pair(pair);
}

void test_pair_set_first(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    first = new_integer(1);
    pair_set_first(pair, first);

    first = pair_first(pair);
    CU_ASSERT_EQUAL(*first, 1);

    second = pair_second(pair);
    CU_ASSERT_PTR_NULL(second);

    first = new_integer(2);
    pair_set_first(pair, first);

    first = pair_first(pair);
    CU_ASSERT_EQUAL(*first, 2);

    second = pair_second(pair);
    CU_ASSERT_PTR_NULL(second);

    delete_pair(pair);
}

void test_pair_set_second(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    second = new_string("one");
    pair_set_second(pair, second);

    first = pair_first(pair);
    CU_ASSERT_PTR_NULL(first);

    second = pair_second(pair);
    CU_ASSERT_STRING_EQUAL(second, "one");

    second = new_string("two");
    pair_set_second(pair, second);

    first = pair_first(pair);
    CU_ASSERT_PTR_NULL(first);

    second = pair_second(pair);
    CU_ASSERT_STRING_EQUAL(second, "two");

    delete_pair(pair);
}

void test_pair_assign(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    first = new_integer(1);
    second = new_string("one");
    pair_set(pair, first, second);

    first = NULL;
    second = NULL;
    pair_assign((void**)&first, (void**)&second, pair);

    CU_ASSERT_EQUAL(*first, 1);
    CU_ASSERT_STRING_EQUAL(second, "one");

    free(first);
    free(second);
}

void test_pair_assign_first_null(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    second = new_string("one");
    pair_set_second(pair, second);

    first = NULL;
    second = NULL;
    pair_assign((void**)&first, (void**)&second, pair);

    CU_ASSERT_PTR_NULL(first);
    CU_ASSERT_STRING_EQUAL(second, "one");

    free(second);
}

void test_pair_assign_second_null(void) {
    Pair* pair = new_pair(&t_integer, &t_string);
    int* first = NULL;
    char* second = NULL;

    first = new_integer(1);
    pair_set_first(pair, first);

    first = NULL;
    second = NULL;
    pair_assign((void**)&first, (void**)&second, pair);

    CU_ASSERT_EQUAL(*first, 1);
    CU_ASSERT_PTR_NULL(second);

    free(first);
}
