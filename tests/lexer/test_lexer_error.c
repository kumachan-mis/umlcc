#include "./test_lexer_error.h"
#include "../../src/lexer/lexer.h"
#include "../testlib/testlib.h"

void test_read_error_character(void);
void test_read_integer_constant_error_empty(void);
void test_read_character_constant_error_empty(void);
void test_read_character_constant_error_newline(void);
void test_read_character_constant_error_escape_sequence(void);
void test_read_string_literal_error_newline(void);
void test_read_string_literal_error_escape_sequence(void);

void run_lexer_error_test(char* input, Error* expected);

CU_Suite* add_test_suite_lexer_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_lexer_error", NULL, NULL);
    CU_ADD_TEST(suite, test_read_error_character);
    CU_ADD_TEST(suite, test_read_integer_constant_error_empty);
    CU_ADD_TEST(suite, test_read_character_constant_error_empty);
    CU_ADD_TEST(suite, test_read_character_constant_error_newline);
    CU_ADD_TEST(suite, test_read_character_constant_error_escape_sequence);
    CU_ADD_TEST(suite, test_read_string_literal_error_newline);
    CU_ADD_TEST(suite, test_read_string_literal_error_escape_sequence);
    return suite;
}

void test_read_error_character(void) {
    char* input = "@variable = 1;";
    Error* expected = new_error("unexpected character '@'\n");

    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void test_read_integer_constant_error_empty(void) {
    char* input = "num = 0x";
    Error* expected = new_error("integer constant digits are empty\n");

    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void test_read_character_constant_error_empty(void) {
    char* input = "ch = '';\n";
    Error* expected = new_error("character constant is empty\n");

    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void test_read_character_constant_error_newline(void) {
    char* input = NULL;
    Error* expected = new_error("newline appeared in character constant\n");

    input = "ch = '\n';\n";
    run_lexer_error_test(input, expected);

    input = "ch = 'a\n';\n";
    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void test_read_character_constant_error_escape_sequence(void) {
    char* input = NULL;
    Error* expected = new_error("invalid escape sequence \\!\n");

    input = "ch = '\\!';";
    run_lexer_error_test(input, expected);

    input = "ch = 'a\\!';";
    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void test_read_string_literal_error_newline(void) {
    char* input = "str = \"\n\";";
    Error* expected = new_error("newline appeared in string literal\n");

    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void test_read_string_literal_error_escape_sequence(void) {
    char* input = "str = \"\\~\";";
    Error* expected = new_error("invalid escape sequence \\~\n");

    run_lexer_error_test(input, expected);

    delete_error(expected);
}

void run_lexer_error_test(char* input, Error* expected) {
    FILE* file_ptr = tmpfile();
    fprintf(file_ptr, "%s", input);
    rewind(file_ptr);

    Lexer* lexer = new_lexer(file_ptr);
    Vector* ret = NULL;
    Error* actual = NULL;
    lexerret_assign(&ret, &actual, lexer_read_ctokens(lexer));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) {
        delete_error(actual);
    }
    delete_lexer(lexer);

    fclose(file_ptr);
}
