#include "./test_lexer.h"
#include "../../src/lexer/lexer.h"
#include "../testlib/testlib.h"

void test_read_function_definition();
void test_read_declaration_without_init();
void test_read_declaration_with_init();
void test_read_assignment_expr();
void test_read_logical_expr();
void test_read_equality_expr();
void test_read_additive_expr();
void test_read_multiplicative_expr();
void test_read_unary_expr();
void test_read_postfix_expr();
void test_read_decimal_integer_constant();
void test_read_octal_integer_constant();
void test_read_hexadecimal_integer_constant();
void test_read_character_constant();
void test_read_octal_character_constant();
void test_read_hexadecimal_character_constant();
void test_read_string_literal();

void run_lexer_test(char* input, Vector* expected);

CU_Suite* add_test_suite_lexer() {
    CU_Suite* suite = CU_add_suite("test_suite_lexer", NULL, NULL);
    CU_ADD_TEST(suite, test_read_function_definition);
    CU_ADD_TEST(suite, test_read_declaration_without_init);
    CU_ADD_TEST(suite, test_read_declaration_with_init);
    CU_ADD_TEST(suite, test_read_assignment_expr);
    CU_ADD_TEST(suite, test_read_decimal_integer_constant);
    CU_ADD_TEST(suite, test_read_octal_integer_constant);
    CU_ADD_TEST(suite, test_read_hexadecimal_integer_constant);
    CU_ADD_TEST(suite, test_read_character_constant);
    CU_ADD_TEST(suite, test_read_octal_character_constant);
    CU_ADD_TEST(suite, test_read_hexadecimal_character_constant);
    CU_ADD_TEST(suite, test_read_string_literal);
    CU_ADD_TEST(suite, test_read_logical_expr);
    CU_ADD_TEST(suite, test_read_equality_expr);
    CU_ADD_TEST(suite, test_read_additive_expr);
    CU_ADD_TEST(suite, test_read_multiplicative_expr);
    CU_ADD_TEST(suite, test_read_unary_expr);
    CU_ADD_TEST(suite, test_read_postfix_expr);
    return suite;
}

void test_read_function_definition() {
    char* input = "int test_read_function_definition() {\n"
                  "    printf(\"OK\\n\");"
                  "    return 0;"
                  "}\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("test_read_function_definition")));
    vector_push(expected, new_ctoken(CTOKEN_LPALEN));
    vector_push(expected, new_ctoken(CTOKEN_RPALEN));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("printf")));
    vector_push(expected, new_ctoken(CTOKEN_LPALEN));
    vector_push(expected, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("OK\n"), 4)));
    vector_push(expected, new_ctoken(CTOKEN_RPALEN));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_without_init() {
    char* input = "typedef int* ptr2int; char* input;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr2int")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("input")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_with_init() {
    char* input = "int int_array[3] = {8, 2, 0};";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("int_array")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACKET));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(expected, new_ctoken(CTOKEN_RBRACKET));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_assignment_expr() {
    char* input = "x86 = 64;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("x86")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 64)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_logical_expr() {
    char* input = "a && xyz; b || _123;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(expected, new_ctoken(CTOKEN_AND_AND));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("xyz")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(expected, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("_123")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_equality_expr() {
    char* input = "flag == 0; flag != 1;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("flag")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("flag")));
    vector_push(expected, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_additive_expr() {
    char* input = "3 + 6; ptr - 1;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(expected, new_ctoken(CTOKEN_PLUS));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 6)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr")));
    vector_push(expected, new_ctoken(CTOKEN_MINUS));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_multiplicative_expr() {
    char* input = "n * 4; m / 8; offset % 2;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("n")));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 4)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("m")));
    vector_push(expected, new_ctoken(CTOKEN_SLASH));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("offset")));
    vector_push(expected, new_ctoken(CTOKEN_PERCENT));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_unary_expr() {
    char* input = "&a; *ptr; !false_flag; !!true_flag;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_AND));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EXCLAM));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("false_flag")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EXCLAM));
    vector_push(expected, new_ctoken(CTOKEN_EXCLAM));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("true_flag")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_postfix_expr() {
    char* input = "arr[10][4]; exec_process(a,  b);";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("arr")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACKET));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 10)));
    vector_push(expected, new_ctoken(CTOKEN_RBRACKET));
    vector_push(expected, new_ctoken(CTOKEN_LBRACKET));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 4)));
    vector_push(expected, new_ctoken(CTOKEN_RBRACKET));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("exec_process")));
    vector_push(expected, new_ctoken(CTOKEN_LPALEN));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(expected, new_ctoken(CTOKEN_RPALEN));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_decimal_integer_constant() {
    char* input = "1 23 456 789 10 2147483647\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 23)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 456)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 789)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 10)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2147483647)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_octal_integer_constant() {
    char* input = "04 076 0123 05 0 017777777777\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 04)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 076)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0123)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 05)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 017777777777)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_hexadecimal_integer_constant() {
    char* input = "0x12 0X034 0XaFd 0X6cB 0x7e 0x598 0X0 0x7FFFFFFF\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0x12)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0X034)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0XaFd)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0X6cB)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0x7e)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0x598)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0X0)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0x7FFFFFFF)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_character_constant() {
    char* input = "'0' 'a' 'xy' '#' "
                  "'\\a' '\\b' '\\f' '\\r' '\\t' '\\v' '\\\"' '\\'' '\\?' '\\\\' '\\r\\n'\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '0')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 'a')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 'x')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '#')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\a')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\b')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\f')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\r')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\t')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\v')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\"')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\'')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\?')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\\')));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, '\r')));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_octal_character_constant() {
    char* input = "'\\0' '\\13' '\\107'\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 013)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0107)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_hexadecimal_character_constant() {
    char* input = "'\\x12' '\\x034' '\\xaF' '\\xcB' '\\x7e' '\\x59' '\\xff' '\\x0'\n";

    // In current implementation, char has the same range of values as signed char
    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0x12)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0x34)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0xaF)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0xcB)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0x7e)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0x59)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0xff)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0x0)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_string_literal() {
    char* input = "\"\" \"str literal\" \"str\\tliteral\"\n";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string(""), 1)));
    vector_push(expected, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("str literal"), 12)));
    vector_push(expected, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("str\tliteral"), 12)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void run_lexer_test(char* input, Vector* expected) {
    FILE* file_ptr = tmpfile();
    fprintf(file_ptr, "%s", input);
    rewind(file_ptr);

    Lexer* lexer = new_lexer(file_ptr);
    Vector* actual = NULL;
    Error* err = NULL;
    lexerret_assign(&actual, &err, lexer_read_ctokens(lexer));

    testlib_assert_ctokens_equal(actual, expected);
    CU_ASSERT_PTR_NULL(err);

    if (actual != NULL) delete_vector(actual);
    delete_lexer(lexer);

    fclose(file_ptr);
}
