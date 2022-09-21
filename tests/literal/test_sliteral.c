#include "./test_sliteral.h"
#include "../../src/literal/sliteral.h"

#include <stdlib.h>

void test_new_sliteral_empty(void);
void test_new_sliteral_without_null(void);
void test_new_sliteral_with_null(void);
void test_sliteral_zero_padding_copy_empty(void);
void test_sliteral_zero_padding_copy_border(void);
void test_sliteral_zero_padding_copy_without_null(void);
void test_sliteral_zero_padding_copy_with_null(void);
void test_sliteral_display_string_normal(void);
void test_sliteral_display_string_escape(void);
void test_sliteral_display_string_octal(void);
void test_sliteral_display_string_mixed(void);
void test_sliteral_display_string_elonged(void);
void test_sliteral_display_string_zero_padding(void);

CU_Suite* add_test_suite_sliteral(void) {
    CU_Suite* suite = CU_add_suite("test_suite_sliteral", NULL, NULL);
    CU_ADD_TEST(suite, test_new_sliteral_empty);
    CU_ADD_TEST(suite, test_new_sliteral_without_null);
    CU_ADD_TEST(suite, test_new_sliteral_with_null);
    CU_ADD_TEST(suite, test_sliteral_zero_padding_copy_empty);
    CU_ADD_TEST(suite, test_sliteral_zero_padding_copy_border);
    CU_ADD_TEST(suite, test_sliteral_zero_padding_copy_without_null);
    CU_ADD_TEST(suite, test_sliteral_zero_padding_copy_with_null);
    CU_ADD_TEST(suite, test_sliteral_display_string_normal);
    CU_ADD_TEST(suite, test_sliteral_display_string_escape);
    CU_ADD_TEST(suite, test_sliteral_display_string_octal);
    CU_ADD_TEST(suite, test_sliteral_display_string_mixed);
    CU_ADD_TEST(suite, test_sliteral_display_string_elonged);
    CU_ADD_TEST(suite, test_sliteral_display_string_zero_padding);
    return suite;
}

void test_new_sliteral_empty(void) {
    char sliteral_const[1] = "";
    int sliteral_size = 1;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            StringLiteral* copied_sliteral = sliteral_copy(sliteral);
            delete_sliteral(sliteral);
            sliteral = copied_sliteral;
        }
        CU_ASSERT_EQUAL(memcmp(sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(sliteral->size, sliteral_size);
    }

    delete_sliteral(sliteral);
}

void test_new_sliteral_without_null(void) {
    char sliteral_const[40] = "test: sliteral copy (without null-char)";
    int sliteral_size = 40;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            StringLiteral* copied_sliteral = sliteral_copy(sliteral);
            delete_sliteral(sliteral);
            sliteral = copied_sliteral;
        }
        CU_ASSERT_EQUAL(memcmp(sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(sliteral->size, sliteral_size);
    }

    delete_sliteral(sliteral);
}

void test_new_sliteral_with_null(void) {
    char sliteral_const[40] = "test: sliteral copy\0(with\0\0\0 null-char)";
    int sliteral_size = 40;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            StringLiteral* copied_sliteral = sliteral_copy(sliteral);
            delete_sliteral(sliteral);
            sliteral = copied_sliteral;
        }
        CU_ASSERT_EQUAL(memcmp(sliteral->value, sliteral_const, sliteral_size), 0);
        CU_ASSERT_EQUAL(sliteral->size, sliteral_size);
    }

    delete_sliteral(sliteral);
}

void test_sliteral_zero_padding_copy_empty(void) {
    char sliteral_const[1] = "";
    int sliteral_size = 1;
    char zero_padding_sliteral_const[5] = "\0\0\0\0";
    int zero_padding_sliteral_size = 5;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_zero_padding_copy_border(void) {
    char sliteral_const[13] = "zero padding";
    int sliteral_size = 13;
    char zero_padding_sliteral_const[12] = "zero padding";
    int zero_padding_sliteral_size = 12;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_zero_padding_copy_without_null(void) {
    char sliteral_const[13] = "zero padding";
    int sliteral_size = 13;
    char zero_padding_sliteral_const[16] = "zero padding\0\0\0";
    int zero_padding_sliteral_size = 16;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_zero_padding_copy_with_null(void) {
    char sliteral_const[13] = "zero\0padding";
    int sliteral_size = 13;
    char zero_padding_sliteral_const[14] = "zero\0padding\0";
    int zero_padding_sliteral_size = 14;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_display_string_normal(void) {
    char sliteral_const[92] = " !#$%&()*+,-./"
                              "0123456789"
                              ":;<=>@"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "[]^_`"
                              "abcdefghijklmnopqrstuvwxy"
                              "z{|}~";
    int sliteral_size = 92;
    char* expected_display_string = "\""
                                    " !#$%&()*+,-./"
                                    "0123456789"
                                    ":;<=>@"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "[]^_`"
                                    "abcdefghijklmnopqrstuvwxy"
                                    "z{|}~"
                                    "\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_escape(void) {
    char sliteral_const[22] = "\? \' \" \\ \a \b \f \n \r \t \v";
    int sliteral_size = 22;
    char* expected_display_string = "\"\\\? \\\' \\\" \\\\ \\a \\b \\f \\n \\r \\t \\v\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_octal(void) {
    char sliteral_const[10] = "\0\1\2\03\04\05\006\007\060";
    int sliteral_size = 10;
    char* expected_display_string = "\"\\000\\001\\002\\003\\004\\005\\006\\a0\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_mixed(void) {
    char sliteral_const[17] = "string\0literal\03\n";
    int sliteral_size = 17;
    char* expected_display_string = "\"string\\000literal\\003\\n\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_elonged(void) {
    char sliteral_const[8] = "\0\1\2\3\4\5\6";
    int sliteral_size = 8;
    char* expected_display_string = "\"\\000\\001\\002\\003\\004\\005\\006\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_zero_padding(void) {
    char sliteral_const[15] = "string literal";
    int sliteral_size = 15;
    int zero_padding_sliteral_size = 18;
    char* expected_display_string = "\"string literal\\000\\000\\000\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* zero_padding_sliteral = sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    char* display_string = sliteral_display_string(zero_padding_sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
    delete_sliteral(zero_padding_sliteral);
}
