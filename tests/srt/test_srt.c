#include "./test_srt.h"
#include "../../src/srt/srt.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_new_srt();
void test_new_dtyped_srt();
void test_new_identifier_srt();
void test_new_integer_srt_int();
void test_new_integer_srt_char();
void test_new_sliteral_srt_empty();
void test_new_sliteral_srt_without_null();
void test_new_sliteral_srt_with_null();

CU_Suite* add_test_suite_srt() {
    CU_Suite* suite = CU_add_suite("test_suite_srt", NULL, NULL);
    CU_add_test(suite, "test_new_srt", test_new_srt);
    CU_add_test(suite, "test_new_dtyped_srt", test_new_dtyped_srt);
    CU_add_test(suite, "test_new_identifier_srt", test_new_identifier_srt);
    CU_add_test(suite, "test_new_integer_srt_int", test_new_integer_srt_int);
    CU_add_test(suite, "test_new_integer_srt_char", test_new_integer_srt_char);
    CU_add_test(suite, "test_new_sliteral_srt_empty", test_new_sliteral_srt_empty);
    CU_add_test(suite, "test_new_sliteral_srt_without_null", test_new_sliteral_srt_without_null);
    CU_add_test(suite, "test_new_sliteral_srt_with_null", test_new_sliteral_srt_with_null);
    return suite;
}

void test_new_srt() {
    Srt* srt = new_srt(SRT_TRAS_UNIT, 2, new_srt(SRT_DECL, 0), new_srt(SRT_FUNC_DEF, 0));
    Srt* child = NULL;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_TRAS_UNIT);
        CU_ASSERT_PTR_NULL(srt->dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, -1);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 2);

        child = vector_at(srt->children, 0);
        CU_ASSERT_EQUAL(child->type, SRT_DECL);
        CU_ASSERT_PTR_NULL(child->dtype);
        CU_ASSERT_PTR_NULL(child->ident_name);
        CU_ASSERT_EQUAL(child->value_int, -1);
        CU_ASSERT_PTR_NULL(child->sliteral);
        CU_ASSERT_EQUAL(vector_size(child->children), 0);

        child = vector_at(srt->children, 1);
        CU_ASSERT_EQUAL(child->type, SRT_FUNC_DEF);
        CU_ASSERT_PTR_NULL(child->dtype);
        CU_ASSERT_PTR_NULL(child->ident_name);
        CU_ASSERT_EQUAL(child->value_int, -1);
        CU_ASSERT_PTR_NULL(child->sliteral);
        CU_ASSERT_EQUAL(vector_size(child->children), 0);
    }

    delete_srt(srt);
}

void test_new_dtyped_srt() {
    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    Dtype* lhs_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* rhs_dtype = new_integer_dtype(DTYPE_INT);

    Srt* srt = new_dtyped_srt(SRT_ADD_EXPR, dtype, 2, new_integer_srt(SRT_INT_EXPR, lhs_dtype, 6),
                              new_integer_srt(SRT_INT_EXPR, rhs_dtype, 3));
    Srt* child = NULL;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            Srt* copied_child = NULL;
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;

            copied_child = vector_at(copied_srt->children, 0);
            lhs_dtype = copied_child->dtype;
            copied_child = vector_at(copied_srt->children, 1);
            rhs_dtype = copied_child->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_ADD_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, -1);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 2);

        child = vector_at(srt->children, 0);
        CU_ASSERT_EQUAL(child->type, SRT_INT_EXPR);
        CU_ASSERT_PTR_EQUAL(child->dtype, lhs_dtype);
        CU_ASSERT_PTR_NULL(child->ident_name);
        CU_ASSERT_EQUAL(child->value_int, 6);
        CU_ASSERT_PTR_NULL(child->sliteral);
        CU_ASSERT_EQUAL(vector_size(child->children), 0);

        child = vector_at(srt->children, 1);
        CU_ASSERT_EQUAL(child->type, SRT_INT_EXPR);
        CU_ASSERT_PTR_EQUAL(child->dtype, rhs_dtype);
        CU_ASSERT_PTR_NULL(child->ident_name);
        CU_ASSERT_EQUAL(child->value_int, 3);
        CU_ASSERT_PTR_NULL(child->sliteral);
        CU_ASSERT_EQUAL(vector_size(child->children), 0);
    }

    delete_srt(srt);
}

void test_new_identifier_srt() {
    Dtype* dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Srt* srt = new_identifier_srt(SRT_IDENT_EXPR, dtype, new_string("ident"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_IDENT_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_STRING_EQUAL(srt->ident_name, "ident");
        CU_ASSERT_EQUAL(srt->value_int, -1);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_integer_srt_int() {
    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    Srt* srt = new_integer_srt(SRT_INT_EXPR, dtype, 6);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_INT_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, 6);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_integer_srt_char() {
    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    Srt* srt = new_integer_srt(SRT_CHAR_EXPR, dtype, 89);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_CHAR_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, 89);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_sliteral_srt_empty() {
    char* sliteral_value = malloc(1 * sizeof(char));
    memcpy(sliteral_value, "", 1 * sizeof(char));

    Dtype* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 1);
    StringLiteral* sliteral = new_sliteral(sliteral_value, 1);
    Srt* srt = new_sliteral_srt(SRT_STRING_EXPR, dtype, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_STRING_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(srt->sliteral->value, "", 1), 0);
        CU_ASSERT_EQUAL(srt->sliteral->size, 1);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_sliteral_srt_without_null() {
    char* sliteral_value = malloc(15 * sizeof(char));
    memcpy(sliteral_value, "string literal", 15 * sizeof(char));

    Dtype* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 15);
    StringLiteral* sliteral = new_sliteral(sliteral_value, 15);
    Srt* srt = new_sliteral_srt(SRT_STRING_EXPR, dtype, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_STRING_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(srt->sliteral->value, "string literal", 15), 0);
        CU_ASSERT_EQUAL(srt->sliteral->size, 15);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_sliteral_srt_with_null() {
    char* sliteral_value = malloc(15 * sizeof(char));
    memcpy(sliteral_value, "string\0literal", 15 * sizeof(char));

    Dtype* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 15);
    StringLiteral* sliteral = new_sliteral(sliteral_value, 15);
    Srt* srt = new_sliteral_srt(SRT_STRING_EXPR, dtype, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_STRING_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(srt->sliteral->value, "string\0literal", 15), 0);
        CU_ASSERT_EQUAL(srt->sliteral->size, 15);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}
