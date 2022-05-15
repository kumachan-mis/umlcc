#include "./test_vector.h"
#include "../src/common/common.h"
#include "../src/vector/vector.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_vector_push();
void test_vector_pop();
void test_vector_fill_all();
void test_vector_fill_noeffect();
void test_vector_fill_expand();
void test_vector_set_success();
void test_vector_set_fail();

CU_Suite* add_test_suite_vector() {
    CU_Suite* suite = CU_add_suite("test_suite_vector", NULL, NULL);
    CU_add_test(suite, "test_vector_push", test_vector_push);
    CU_add_test(suite, "test_vector_pop", test_vector_pop);
    CU_add_test(suite, "test_vector_fill_all", test_vector_fill_all);
    CU_add_test(suite, "test_vector_fill_noeffect", test_vector_fill_noeffect);
    CU_add_test(suite, "test_vector_fill_expand", test_vector_fill_expand);
    CU_add_test(suite, "test_vector_set_success", test_vector_set_success);
    CU_add_test(suite, "test_vector_set_fail", test_vector_set_fail);
    return suite;
}

void test_vector_push() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;
    CU_ASSERT_EQUAL(vector_size(vector), 0);

    item = new_integer(2);
    vector_push(vector, item);
    CU_ASSERT_EQUAL(vector_size(vector), 1);

    item = new_integer(3);
    vector_push(vector, item);
    CU_ASSERT_EQUAL(vector_size(vector), 2);

    item = new_integer(5);
    vector_push(vector, item);
    CU_ASSERT_EQUAL(vector_size(vector), 3);

    item = new_integer(6);
    vector_push(vector, item);
    CU_ASSERT_EQUAL(vector_size(vector), 4);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 2);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 3);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, 5);

    item = vector_at(vector, 3);
    CU_ASSERT_EQUAL(*item, 6);

    CU_ASSERT_EQUAL(vector_size(vector), 4);
    delete_vector(vector);
}

void test_vector_pop() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    item = new_integer(-3);
    vector_push(vector, item);

    item = new_integer(-2);
    vector_push(vector, item);

    item = new_integer(0);
    vector_push(vector, item);

    CU_ASSERT_EQUAL(vector_size(vector), 3);

    item = vector_pop(vector);
    CU_ASSERT_EQUAL(*item, 0);
    CU_ASSERT_EQUAL(vector_size(vector), 2);
    free(item);

    item = vector_pop(vector);
    CU_ASSERT_EQUAL(*item, -2);
    CU_ASSERT_EQUAL(vector_size(vector), 1);
    free(item);

    item = vector_pop(vector);
    CU_ASSERT_EQUAL(*item, -3);
    CU_ASSERT_EQUAL(vector_size(vector), 0);
    free(item);

    delete_vector(vector);
}

void test_vector_fill_all() {
    Vector* vector = new_vector(&t_integer);

    int* item = new_integer(0);
    vector_fill(vector, 10, item);
    free(item);

    for (int i = 0; i < 10; i++) {
        item = vector_at(vector, i);
        CU_ASSERT_EQUAL(*item, 0);
    }
    CU_ASSERT_PTR_NULL(vector_at(vector, 10));

    delete_vector(vector);
}

void test_vector_fill_noeffect() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    for (int i = -5; i < 0; i++) {
        vector_push(vector, new_integer(i));
    }

    item = new_integer(7);
    vector_fill(vector, 3, item);
    free(item);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, -5);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, -4);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, -3);

    item = vector_at(vector, 3);
    CU_ASSERT_EQUAL(*item, -2);

    item = vector_at(vector, 4);
    CU_ASSERT_EQUAL(*item, -1);

    item = vector_at(vector, 5);
    CU_ASSERT_PTR_NULL(item);

    delete_vector(vector);
}

void test_vector_fill_expand() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    for (int i = -5; i < -2; i++) {
        vector_push(vector, new_integer(i));
    }

    item = new_integer(7);
    vector_fill(vector, 5, item);
    free(item);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, -5);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, -4);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, -3);

    item = vector_at(vector, 3);
    CU_ASSERT_EQUAL(*item, 7);

    item = vector_at(vector, 4);
    CU_ASSERT_EQUAL(*item, 7);

    item = vector_at(vector, 5);
    CU_ASSERT_PTR_NULL(item);

    delete_vector(vector);
}

void test_vector_set_success() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    int* zero = new_integer(0);
    vector_fill(vector, 4, zero);
    free(zero);

    item = new_integer(1);
    CU_ASSERT_TRUE(vector_set(vector, 2, item));

    item = new_integer(2);
    CU_ASSERT_TRUE(vector_set(vector, 1, item));

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 0);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 2);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, 1);

    item = vector_at(vector, 3);
    CU_ASSERT_EQUAL(*item, 0);

    item = vector_at(vector, 4);
    CU_ASSERT_PTR_NULL(item);

    delete_vector(vector);
}

void test_vector_set_fail() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    int* zero = new_integer(0);
    vector_fill(vector, 4, zero);
    free(zero);

    item = new_integer(1);
    CU_ASSERT_FALSE(vector_set(vector, -1, item));
    free(item);

    item = new_integer(2);
    CU_ASSERT_FALSE(vector_set(vector, 4, item));
    free(item);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 0);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 0);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, 0);

    item = vector_at(vector, 3);
    CU_ASSERT_EQUAL(*item, 0);

    item = vector_at(vector, 4);
    CU_ASSERT_PTR_NULL(item);

    delete_vector(vector);
}
