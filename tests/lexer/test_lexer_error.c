#include "./test_lexer_error.h"
#include "../../src/lexer/lexer.h"
#include "../testlib/testlib.h"

void test_read_unexpected_character_error();
void test_read_empty_character_constant_error();
void test_read_newline_character_constant_error();
void test_read_invalid_escape_sequence_character_constant_error();
void test_read_newline_string_literal_error();
void test_read_invalid_escape_sequence_string_literal_error();

void run_lexer_error_test(const char* __restrict__ input, const char* __restrict__ message);

CU_Suite* add_test_suite_lexer_error() {
    CU_Suite* suite = CU_add_suite("test_suite_lexer_error", NULL, NULL);
    CU_ADD_TEST(suite, test_read_unexpected_character_error);
    CU_ADD_TEST(suite, test_read_empty_character_constant_error);
    CU_ADD_TEST(suite, test_read_newline_character_constant_error);
    CU_ADD_TEST(suite, test_read_invalid_escape_sequence_character_constant_error);
    CU_ADD_TEST(suite, test_read_newline_string_literal_error);
    CU_ADD_TEST(suite, test_read_invalid_escape_sequence_string_literal_error);
    return suite;
}

void test_read_unexpected_character_error() {
    const char* input = "@variable = 1;\n";
    const char* message = "Error: unexpected character\n";
    run_lexer_error_test(input, message);
}

void test_read_empty_character_constant_error() {
    const char* input = "ch = '';\n";
    const char* message = "Error: character constant is empty\n";
    run_lexer_error_test(input, message);
}

void test_read_newline_character_constant_error() {
    const char* input = NULL;
    const char* message = "Error: newline appeared in character constant\n";

    input = "ch = '\n';\n";
    run_lexer_error_test(input, message);

    input = "ch = 'a\n';\n";
    run_lexer_error_test(input, message);
}

void test_read_invalid_escape_sequence_character_constant_error() {
    const char* input = NULL;
    const char* message = "Error: invalid escape sequence\n";

    input = "ch = '\\!';\n";
    run_lexer_error_test(input, message);

    input = "ch = 'a\\!';\n";
    run_lexer_error_test(input, message);
}

void test_read_newline_string_literal_error() {
    const char* input = "str = \"\n\";\n";
    const char* message = "Error: newline appeared in string literal\n";
    run_lexer_error_test(input, message);
}

void test_read_invalid_escape_sequence_string_literal_error() {
    const char* input = "str = \"\\~\";\n";
    const char* message = "Error: invalid escape sequence\n";
    run_lexer_error_test(input, message);
}

void run_lexer_error_test(const char* __restrict__ input, const char* __restrict__ message) {
    FILE* file_ptr = tmpfile();
    fprintf(file_ptr, "%s", input);
    rewind(file_ptr);

    Lexer* lexer = new_lexer(file_ptr);
    LexerReturn* lexerret = lexer_read_ctokens(lexer);
    Vector* actual = lexerret->ctokens;
    Error* err = lexerret->err;
    lexerret_close(lexerret);

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_lexer(lexer);

    fclose(file_ptr);
}
