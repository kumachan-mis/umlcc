#include "./test_lexer_error.h"
#include "../../src/lexer/lexer.h"
#include "../testlib/testlib.h"

void test_read_error_character();
void test_read_character_constant_error_empty();
void test_read_character_constant_error_newline();
void test_read_character_constant_error_escape_sequence();
void test_read_string_literal_error_newline();
void test_read_string_literal_error_escape_sequence();

void run_lexer_error_test(const char* __restrict__ input, const char* __restrict__ message);

CU_Suite* add_test_suite_lexer_error() {
    CU_Suite* suite = CU_add_suite("test_suite_lexer_error", NULL, NULL);
    CU_ADD_TEST(suite, test_read_error_character);
    CU_ADD_TEST(suite, test_read_character_constant_error_empty);
    CU_ADD_TEST(suite, test_read_character_constant_error_newline);
    CU_ADD_TEST(suite, test_read_character_constant_error_escape_sequence);
    CU_ADD_TEST(suite, test_read_string_literal_error_newline);
    CU_ADD_TEST(suite, test_read_string_literal_error_escape_sequence);
    return suite;
}

void test_read_error_character() {
    const char* input = "@variable = 1;\n";
    const char* message = "Error: unexpected character '@'\n";
    run_lexer_error_test(input, message);
}

void test_read_character_constant_error_empty() {
    const char* input = "ch = '';\n";
    const char* message = "Error: character constant is empty\n";
    run_lexer_error_test(input, message);
}

void test_read_character_constant_error_newline() {
    const char* input = NULL;
    const char* message = "Error: newline appeared in character constant\n";

    input = "ch = '\n';\n";
    run_lexer_error_test(input, message);

    input = "ch = 'a\n';\n";
    run_lexer_error_test(input, message);
}

void test_read_character_constant_error_escape_sequence() {
    const char* input = NULL;
    const char* message = "Error: invalid escape sequence \\!\n";

    input = "ch = '\\!';\n";
    run_lexer_error_test(input, message);

    input = "ch = 'a\\!';\n";
    run_lexer_error_test(input, message);
}

void test_read_string_literal_error_newline() {
    const char* input = "str = \"\n\";\n";
    const char* message = "Error: newline appeared in string literal\n";
    run_lexer_error_test(input, message);
}

void test_read_string_literal_error_escape_sequence() {
    const char* input = "str = \"\\~\";\n";
    const char* message = "Error: invalid escape sequence \\~\n";
    run_lexer_error_test(input, message);
}

void run_lexer_error_test(const char* __restrict__ input, const char* __restrict__ message) {
    FILE* file_ptr = tmpfile();
    fprintf(file_ptr, "%s", input);
    rewind(file_ptr);

    Lexer* lexer = new_lexer(file_ptr);
    Vector* actual = NULL;
    Error* err = NULL;
    lexerret_assign(&actual, &err, lexer_read_ctokens(lexer));

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_lexer(lexer);

    fclose(file_ptr);
}
