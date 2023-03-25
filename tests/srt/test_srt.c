#include "./test_srt.h"
#include "../../src/srt/srt.h"

#include <stdlib.h>

void test_new_srt(void);
void test_new_dtyped_srt(void);
void test_new_identifier_srt(void);
void test_new_iliteral_srt_int(void);
void test_new_iliteral_srt_char(void);
void test_new_sliteral_srt(void);

CU_Suite* add_test_suite_srt(void) {
    CU_Suite* suite = CU_add_suite("test_suite_srt", NULL, NULL);
    CU_ADD_TEST(suite, test_new_srt);
    CU_ADD_TEST(suite, test_new_dtyped_srt);
    CU_ADD_TEST(suite, test_new_identifier_srt);
    CU_ADD_TEST(suite, test_new_iliteral_srt_int);
    CU_ADD_TEST(suite, test_new_iliteral_srt_char);
    CU_ADD_TEST(suite, test_new_sliteral_srt);
    return suite;
}

void test_new_srt(void) {
    Srt* srt = new_srt(SRT_TRAS_UNIT, 2, new_srt(SRT_DECL, 0), new_srt(SRT_FUNC_DEF, 0));
    Srt* child_srt = NULL;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_TRAS_UNIT);
        CU_ASSERT_PTR_NULL(srt->dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_PTR_NULL(srt->iliteral);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 2);

        child_srt = vector_at(srt->children, 0);
        CU_ASSERT_EQUAL(child_srt->type, SRT_DECL);
        CU_ASSERT_PTR_NULL(child_srt->dtype);
        CU_ASSERT_PTR_NULL(child_srt->ident_name);
        CU_ASSERT_PTR_NULL(child_srt->iliteral);
        CU_ASSERT_PTR_NULL(child_srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(child_srt->children), 0);

        child_srt = vector_at(srt->children, 1);
        CU_ASSERT_EQUAL(child_srt->type, SRT_FUNC_DEF);
        CU_ASSERT_PTR_NULL(child_srt->dtype);
        CU_ASSERT_PTR_NULL(child_srt->ident_name);
        CU_ASSERT_PTR_NULL(child_srt->iliteral);
        CU_ASSERT_PTR_NULL(child_srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(child_srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_dtyped_srt(void) {
    DType* dtype = new_integer_dtype(DTYPE_INT);
    DType* lhs_dtype = new_integer_dtype(DTYPE_INT);
    DType* rhs_dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* lhs_iliteral = new_signed_iliteral(INTEGER_INT, 6);
    IntegerLiteral* rhs_iliteral = new_signed_iliteral(INTEGER_INT, 3);
    Srt* srt = new_dtyped_srt(SRT_ADD_EXPR, dtype, 2, new_iliteral_srt(SRT_INT_EXPR, lhs_dtype, lhs_iliteral),
                              new_iliteral_srt(SRT_INT_EXPR, rhs_dtype, rhs_iliteral));
    Srt* child_srt = NULL;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            Srt* copied_child = NULL;
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;

            copied_child = vector_at(copied_srt->children, 0);
            lhs_dtype = copied_child->dtype;
            lhs_iliteral = copied_child->iliteral;
            copied_child = vector_at(copied_srt->children, 1);
            rhs_dtype = copied_child->dtype;
            rhs_iliteral = copied_child->iliteral;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_ADD_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_PTR_NULL(srt->iliteral);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 2);

        child_srt = vector_at(srt->children, 0);
        CU_ASSERT_EQUAL(child_srt->type, SRT_INT_EXPR);
        CU_ASSERT_PTR_EQUAL(child_srt->dtype, lhs_dtype);
        CU_ASSERT_PTR_NULL(child_srt->ident_name);
        CU_ASSERT_EQUAL(child_srt->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(child_srt->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(child_srt->iliteral->signed_value, 6);
        CU_ASSERT_EQUAL(child_srt->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(child_srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(child_srt->children), 0);

        child_srt = vector_at(srt->children, 1);
        CU_ASSERT_EQUAL(child_srt->type, SRT_INT_EXPR);
        CU_ASSERT_PTR_EQUAL(child_srt->dtype, rhs_dtype);
        CU_ASSERT_PTR_NULL(child_srt->ident_name);
        CU_ASSERT_EQUAL(child_srt->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(child_srt->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(child_srt->iliteral->signed_value, 3);
        CU_ASSERT_EQUAL(child_srt->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(child_srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(child_srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_identifier_srt(void) {
    DType* dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
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
        CU_ASSERT_PTR_NULL(srt->iliteral);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_iliteral_srt_int(void) {
    DType* dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 6);
    Srt* srt = new_iliteral_srt(SRT_INT_EXPR, dtype, iliteral);

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
        CU_ASSERT_EQUAL(srt->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(srt->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(srt->iliteral->signed_value, 6);
        CU_ASSERT_EQUAL(srt->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_iliteral_srt_char(void) {
    DType* dtype = new_integer_dtype(DTYPE_INT);
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 89);
    Srt* srt = new_iliteral_srt(SRT_CHAR_EXPR, dtype, iliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Srt* copied_srt = srt_copy(srt);
            delete_srt(srt);
            srt = copied_srt;
            dtype = copied_srt->dtype;
            iliteral = copied_srt->iliteral;
        }
        CU_ASSERT_EQUAL(srt->type, SRT_CHAR_EXPR);
        CU_ASSERT_PTR_EQUAL(srt->dtype, dtype);
        CU_ASSERT_PTR_NULL(srt->ident_name);
        CU_ASSERT_EQUAL(srt->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(srt->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(srt->iliteral->signed_value, 89);
        CU_ASSERT_EQUAL(srt->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(srt->sliteral);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}

void test_new_sliteral_srt(void) {
    char sliteral_const[15] = "string literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    DType* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size);
    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
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
        CU_ASSERT_PTR_NULL(srt->iliteral);
        CU_ASSERT_EQUAL(memcmp(srt->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(srt->sliteral->size, sliteral_size);
        CU_ASSERT_EQUAL(vector_size(srt->children), 0);
    }

    delete_srt(srt);
}
