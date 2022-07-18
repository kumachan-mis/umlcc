#include "./test_vector.h"
#include "../../src/common/type.h"
#include "../../src/vector/vector.h"

#include <stdlib.h>

void test_vector_copy();
void test_vector_push();
void test_vector_pop();
void test_vector_fill_all();
void test_vector_fill_noeffect();
void test_vector_fill_expand();
void test_vector_set_success();
void test_vector_set_fail();
void test_vector_erase_success();
void test_vector_erase_fail();
void test_vector_extend();
void test_vector_extend_empty();

CU_Suite* add_test_suite_vector() {
    CU_Suite* suite = CU_add_suite("test_suite_vector", NULL, NULL);
    CU_add_test(suite, "test_vector_copy", test_vector_copy);
    CU_add_test(suite, "test_vector_push", test_vector_push);
    CU_add_test(suite, "test_vector_pop", test_vector_pop);
    CU_add_test(suite, "test_vector_fill_all", test_vector_fill_all);
    CU_add_test(suite, "test_vector_fill_noeffect", test_vector_fill_noeffect);
    CU_add_test(suite, "test_vector_fill_expand", test_vector_fill_expand);
    CU_add_test(suite, "test_vector_set_success", test_vector_set_success);
    CU_add_test(suite, "test_vector_set_fail", test_vector_set_fail);
    CU_add_test(suite, "test_vector_erase_success", test_vector_erase_success);
    CU_add_test(suite, "test_vector_erase_fail", test_vector_erase_fail);
    CU_add_test(suite, "test_vector_extend", test_vector_extend);
    CU_add_test(suite, "test_vector_extend_empty", test_vector_extend_empty);
    return suite;
}

void test_vector_copy() {
    Vector* vector = new_vector(&t_string);
    char* item = NULL;

    item = new_string("abc");
    vector_push(vector, item);

    item = new_string("string");
    vector_push(vector, item);

    item = new_string("test");
    vector_push(vector, item);

    Vector* copied_vector = vector_copy(vector);
    delete_vector(vector);

    CU_ASSERT_EQUAL(vector_size(copied_vector), 3);

    item = vector_at(copied_vector, 0);
    CU_ASSERT_STRING_EQUAL(item, "abc");

    item = vector_at(copied_vector, 1);
    CU_ASSERT_STRING_EQUAL(item, "string");

    item = vector_at(copied_vector, 2);
    CU_ASSERT_STRING_EQUAL(item, "test");

    delete_vector(copied_vector);
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

void test_vector_erase_success() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    item = new_integer(4);
    vector_push(vector, item);

    item = new_integer(5);
    vector_push(vector, item);

    item = new_integer(2);
    vector_push(vector, item);

    vector_erase(vector, 2);

    CU_ASSERT_EQUAL(vector_size(vector), 2);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 4);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 5);

    vector_erase(vector, 0);

    CU_ASSERT_EQUAL(vector_size(vector), 1);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 5);

    delete_vector(vector);
}

void test_vector_erase_fail() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    item = new_integer(4);
    vector_push(vector, item);

    item = new_integer(5);
    vector_push(vector, item);

    item = new_integer(2);
    vector_push(vector, item);

    vector_erase(vector, -1);
    vector_erase(vector, 3);

    CU_ASSERT_EQUAL(vector_size(vector), 3);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 4);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 5);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, 2);

    delete_vector(vector);
}

void test_vector_extend() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    item = new_integer(1);
    vector_push(vector, item);

    item = new_integer(3);
    vector_push(vector, item);

    item = new_integer(5);
    vector_push(vector, item);

    Vector* other = new_vector(&t_integer);

    item = new_integer(2);
    vector_push(vector, item);

    item = new_integer(4);
    vector_push(vector, item);

    vector_extend(vector, other);

    CU_ASSERT_EQUAL(vector_size(vector), 5);
    CU_ASSERT_EQUAL(vector_size(other), 0);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 1);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 3);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, 5);

    item = vector_at(vector, 3);
    CU_ASSERT_EQUAL(*item, 2);

    item = vector_at(vector, 4);
    CU_ASSERT_EQUAL(*item, 4);

    delete_vector(vector);
    delete_vector(other);
}

void test_vector_extend_empty() {
    Vector* vector = new_vector(&t_integer);
    int* item = NULL;

    item = new_integer(1);
    vector_push(vector, item);

    item = new_integer(2);
    vector_push(vector, item);

    item = new_integer(4);
    vector_push(vector, item);

    Vector* other = new_vector(&t_integer);

    vector_extend(vector, other);

    CU_ASSERT_EQUAL(vector_size(vector), 3);
    CU_ASSERT_EQUAL(vector_size(other), 0);

    item = vector_at(vector, 0);
    CU_ASSERT_EQUAL(*item, 1);

    item = vector_at(vector, 1);
    CU_ASSERT_EQUAL(*item, 2);

    item = vector_at(vector, 2);
    CU_ASSERT_EQUAL(*item, 4);

    delete_vector(vector);
    delete_vector(other);
}