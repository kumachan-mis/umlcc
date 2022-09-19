#include "./test_ctoken.h"
#include "../../src/ctoken/ctoken.h"

#include <stdlib.h>

void test_new_ctoken();
void test_new_identifier_ctoken();
void test_new_iliteral_ctoken_int();
void test_new_iliteral_ctoken_char();
void test_new_sliteral_ctoken();

CU_Suite* add_test_suite_ctoken() {
    CU_Suite* suite = CU_add_suite("test_suite_ctoken", NULL, NULL);
    CU_ADD_TEST(suite, test_new_ctoken);
    CU_ADD_TEST(suite, test_new_identifier_ctoken);
    CU_ADD_TEST(suite, test_new_iliteral_ctoken_int);
    CU_ADD_TEST(suite, test_new_iliteral_ctoken_char);
    CU_ADD_TEST(suite, test_new_sliteral_ctoken);
    return suite;
}

void test_new_ctoken() {
    CToken* ctoken = new_ctoken(CTOKEN_SEMICOLON);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_SEMICOLON);
        CU_ASSERT_PTR_NULL(ctoken->ident_name);
        CU_ASSERT_PTR_NULL(ctoken->iliteral);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_identifier_ctoken() {
    CToken* ctoken = new_identifier_ctoken(CTOKEN_IDENT, new_string("ident"));

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_IDENT);
        CU_ASSERT_STRING_EQUAL(ctoken->ident_name, "ident");
        CU_ASSERT_PTR_NULL(ctoken->iliteral);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_iliteral_ctoken_int() {
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 6);
    CToken* ctoken = new_iliteral_ctoken(CTOKEN_INT, iliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_INT);
        CU_ASSERT_PTR_NULL(ctoken->ident_name);
        CU_ASSERT_EQUAL(ctoken->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(ctoken->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ctoken->iliteral->signed_value, 6);
        CU_ASSERT_EQUAL(ctoken->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_iliteral_ctoken_char() {
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 89);
    CToken* ctoken = new_iliteral_ctoken(CTOKEN_CHAR, iliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_CHAR);
        CU_ASSERT_PTR_NULL(ctoken->ident_name);
        CU_ASSERT_EQUAL(ctoken->iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(ctoken->iliteral->is_unsigned);
        CU_ASSERT_EQUAL(ctoken->iliteral->signed_value, 89);
        CU_ASSERT_EQUAL(ctoken->iliteral->unsigned_value, 0ULL);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_sliteral_ctoken() {
    char* sliteral_const = "string literal";
    int sliteral_size = 15;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    CToken* ctoken = new_sliteral_ctoken(CTOKEN_STRING, sliteral);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_STRING);
        CU_ASSERT_PTR_NULL(ctoken->ident_name);
        CU_ASSERT_PTR_NULL(ctoken->iliteral);
        CU_ASSERT_EQUAL(memcmp(ctoken->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(ctoken->sliteral->size, sliteral_size);
    }

    delete_ctoken(ctoken);
}
