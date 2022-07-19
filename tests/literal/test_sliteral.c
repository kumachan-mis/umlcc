#include "./test_sliteral.h"
#include "../../src/literal/sliteral.h"

#include <stdlib.h>

void test_sliteral_copy_empty();
void test_sliteral_copy_without_null();
void test_sliteral_copy_with_null();
void test_sliteral_zero_padding_copy_empty();
void test_sliteral_zero_padding_copy_without_null();
void test_sliteral_zero_padding_copy_with_null();
void test_sliteral_display_string_normal();
void test_sliteral_display_string_escape();
void test_sliteral_display_string_octal();
void test_sliteral_display_string_mixed();
void test_sliteral_display_string_elonged();
void test_sliteral_display_string_zero_padding();

CU_Suite* add_test_suite_sliteral() {
    CU_Suite* suite = CU_add_suite("test_suite_sliteral", NULL, NULL);
    CU_ADD_TEST(suite, test_sliteral_copy_empty);
    CU_ADD_TEST(suite, test_sliteral_copy_without_null);
    CU_ADD_TEST(suite, test_sliteral_copy_with_null);
    CU_ADD_TEST(suite, test_sliteral_zero_padding_copy_empty);
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

void test_sliteral_copy_empty() {
    const char* sliteral_const = "";
    const int sliteral_size = 1;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_copy(sliteral);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, sliteral_const, sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_copy_without_null() {
    const char* sliteral_const = "test: sliteral copy (without null-char)";
    const int sliteral_size = 40;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_copy(sliteral);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, sliteral_const, sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_copy_with_null() {
    const char* sliteral_const = "test: sliteral copy\0(with\0\0\0 null-char)";
    const int sliteral_size = 40;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral = sliteral_copy(sliteral);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(memcmp(copied_sliteral->value, sliteral_const, sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_zero_padding_copy_empty() {
    const char* sliteral_const = "";
    const int sliteral_size = 1;
    const char* zero_padding_sliteral_const = "\0\0\0\0";
    const int zero_padding_sliteral_size = 5;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral =
        sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(
        memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_zero_padding_copy_without_null() {
    const char* sliteral_const = "zero padding";
    const int sliteral_size = 13;
    const char* zero_padding_sliteral_const = "zero padding\0\0\0";
    const int zero_padding_sliteral_size = 16;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral =
        sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(
        memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_zero_padding_copy_with_null() {
    const char* sliteral_const = "zero\0padding";
    const int sliteral_size = 13;
    const char* zero_padding_sliteral_const = "zero\0padding\0";
    const int zero_padding_sliteral_size = 14;

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* copied_sliteral =
        sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    delete_sliteral(sliteral);

    CU_ASSERT_EQUAL(
        memcmp(copied_sliteral->value, zero_padding_sliteral_const, zero_padding_sliteral_size), 0);
    CU_ASSERT_EQUAL(copied_sliteral->size, zero_padding_sliteral_size);

    delete_sliteral(copied_sliteral);
}

void test_sliteral_display_string_normal() {
    const char* sliteral_const = " !#$%&()*+,-./"
                                 "0123456789"
                                 ":;<=>@"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "[]^_`"
                                 "abcdefghijklmnopqrstuvwxy"
                                 "z{|}~";
    const int sliteral_size = 92;
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

void test_sliteral_display_string_escape() {
    const char* sliteral_const = "\? \' \" \\ \a \b \f \n \r \t \v";
    const int sliteral_size = 22;
    char* expected_display_string = "\"\\\? \\\' \\\" \\\\ \\a \\b \\f \\n \\r \\t \\v\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_octal() {
    const char* sliteral_const = "\0\1\2\03\04\05\006\007\060";
    const int sliteral_size = 10;
    char* expected_display_string = "\"\\000\\001\\002\\003\\004\\005\\006\\a0\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_mixed() {
    const char* sliteral_const = "string\0literal\03\n";
    const int sliteral_size = 17;
    char* expected_display_string = "\"string\\000literal\\003\\n\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_elonged() {
    const char* sliteral_const = "\0\1\2\3\4\5\6";
    const int sliteral_size = 8;
    char* expected_display_string = "\"\\000\\001\\002\\003\\004\\005\\006\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    char* display_string = sliteral_display_string(sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
}

void test_sliteral_display_string_zero_padding() {
    const char* sliteral_const = "string literal";
    const int sliteral_size = 15;
    const int zero_padding_sliteral_size = 18;
    char* expected_display_string = "\"string literal\\000\\000\\000\\000\"";

    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* zero_padding_sliteral =
        sliteral_zero_padding_copy(sliteral, zero_padding_sliteral_size);
    char* display_string = sliteral_display_string(zero_padding_sliteral);

    CU_ASSERT_STRING_EQUAL(display_string, expected_display_string);

    free(display_string);
    delete_sliteral(sliteral);
    delete_sliteral(zero_padding_sliteral);
}
