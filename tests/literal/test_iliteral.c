#include "./test_iliteral.h"
#include "../../src/literal/iliteral.h"

#include <stdlib.h>

void test_new_signed_iliteral();
void test_new_unsigned_iliteral();
void test_iliteral_display_signed();
void test_iliteral_display_unsigned();

CU_Suite* add_test_suite_iliteral() {
    CU_Suite* suite = CU_add_suite("test_suite_iliteral", NULL, NULL);
    CU_ADD_TEST(suite, test_new_signed_iliteral);
    CU_ADD_TEST(suite, test_new_unsigned_iliteral);
    CU_ADD_TEST(suite, test_iliteral_display_signed);
    CU_ADD_TEST(suite, test_iliteral_display_unsigned);
    return suite;
}

void test_new_signed_iliteral() {
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, 45);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            IntegerLiteral* copied_iliteral = iliteral_copy(iliteral);
            delete_iliteral(iliteral);
            iliteral = copied_iliteral;
        }
        CU_ASSERT_EQUAL(iliteral->type, INTEGER_INT);
        CU_ASSERT_FALSE(iliteral->is_unsigned);
        CU_ASSERT_EQUAL(iliteral->signed_value, 45);
        CU_ASSERT_EQUAL(iliteral->unsigned_value, 0ULL);
    }

    delete_iliteral(iliteral);
}

void test_new_unsigned_iliteral() {
    IntegerLiteral* iliteral = new_unsigned_iliteral(INTEGER_LONG, 2147483648U);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            IntegerLiteral* copied_iliteral = iliteral_copy(iliteral);
            delete_iliteral(iliteral);
            iliteral = copied_iliteral;
        }
        CU_ASSERT_EQUAL(iliteral->type, INTEGER_LONG);
        CU_ASSERT_TRUE(iliteral->is_unsigned);
        CU_ASSERT_EQUAL(iliteral->signed_value, -1LL);
        CU_ASSERT_EQUAL(iliteral->unsigned_value, 2147483648ULL);
    }

    delete_iliteral(iliteral);
}

void test_iliteral_display_signed() {
    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, -45);
    char* display_string = iliteral_display_string(iliteral);

    CU_ASSERT_STRING_EQUAL(display_string, "-45");

    free(display_string);
    delete_iliteral(iliteral);
}

void test_iliteral_display_unsigned() {
    IntegerLiteral* iliteral = new_unsigned_iliteral(INTEGER_LONG, 2147483648U);
    char* display_string = iliteral_display_string(iliteral);

    CU_ASSERT_STRING_EQUAL(display_string, "2147483648");

    free(display_string);
    delete_iliteral(iliteral);
}
