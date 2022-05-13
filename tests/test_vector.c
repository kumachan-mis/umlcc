#include "./test_vector.h"
#include "../src/vector/vector.h"
#include "../src/common/common.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_vector_push();
void test_vector_pop();

CU_Suite* add_test_suite_vector() {
    CU_Suite* suite = CU_add_suite("test_suite_vector", NULL, NULL);
    CU_add_test(suite, "test_vector_push", test_vector_push);
    CU_add_test(suite, "test_vector_pop", test_vector_pop);
    return suite;
}

void test_vector_push() {
    Vector* vector = new_vector();
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
    delete_vector(vector, free);
}

void test_vector_pop() {
    Vector* vector = new_vector();
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

    delete_vector(vector, free);
}
