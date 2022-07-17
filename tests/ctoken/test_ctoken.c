#include "./test_ctoken.h"
#include "../../src/ctoken/ctoken.h"

#include <stdlib.h>

void test_new_ctoken();
void test_new_identifier_ctoken();
void test_new_integer_ctoken_int();
void test_new_integer_ctoken_char();
void test_new_sliteral_ctoken();

CU_Suite* add_test_suite_ctoken() {
    CU_Suite* suite = CU_add_suite("test_suite_ctoken", NULL, NULL);
    CU_add_test(suite, "test_new_ctoken", test_new_ctoken);
    CU_add_test(suite, "test_new_identifier_ctoken", test_new_identifier_ctoken);
    CU_add_test(suite, "test_new_integer_ctoken_int", test_new_integer_ctoken_int);
    CU_add_test(suite, "test_new_integer_ctoken_char", test_new_integer_ctoken_char);
    CU_add_test(suite, "test_new_sliteral_ctoken", test_new_sliteral_ctoken);
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
        CU_ASSERT_EQUAL(ctoken->value_int, -1);
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
        CU_ASSERT_EQUAL(ctoken->value_int, -1);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_integer_ctoken_int() {
    CToken* ctoken = new_integer_ctoken(CTOKEN_INT, 6);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_INT);
        CU_ASSERT_PTR_NULL(ctoken->ident_name);
        CU_ASSERT_EQUAL(ctoken->value_int, 6);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_integer_ctoken_char() {
    CToken* ctoken = new_integer_ctoken(CTOKEN_CHAR, 89);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            CToken* copied_ctoken = ctoken_copy(ctoken);
            delete_ctoken(ctoken);
            ctoken = copied_ctoken;
        }
        CU_ASSERT_EQUAL(ctoken->type, CTOKEN_CHAR);
        CU_ASSERT_PTR_NULL(ctoken->ident_name);
        CU_ASSERT_EQUAL(ctoken->value_int, 89);
        CU_ASSERT_PTR_NULL(ctoken->sliteral);
    }

    delete_ctoken(ctoken);
}

void test_new_sliteral_ctoken() {
    const char* sliteral_const = "string literal";
    const int sliteral_size = 15;

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
        CU_ASSERT_EQUAL(ctoken->value_int, -1);
        CU_ASSERT_EQUAL(memcmp(ctoken->sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(ctoken->sliteral->size, sliteral_size);
    }

    delete_ctoken(ctoken);
}
