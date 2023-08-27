#include "./test_lexer.h"
#include "../../src/lexer/lexer.h"
#include "../testlib/testlib.h"

void test_read_function_definition(void);
void test_read_declaration_integer_without_init(void);
void test_read_declaration_pointer_without_init(void);
void test_read_declaration_aggregate_without_init(void);
void test_read_declaration_function_without_init(void);
void test_read_declaration_int_with_init(void);
void test_read_declaration_long_with_init(void);
void test_read_declaration_long_long_with_init(void);
void test_read_declaration_pointer_with_init(void);
void test_read_declaration_aggregate_with_init(void);
void test_read_assignment_expr(void);
void test_read_logical_expr(void);
void test_read_equality_expr(void);
void test_read_relational_expr(void);
void test_read_additive_expr(void);
void test_read_multiplicative_expr(void);
void test_read_unary_expr(void);
void test_read_postfix_expr(void);
void test_read_decimal_integer_constant(void);
void test_read_octal_integer_constant(void);
void test_read_hexadecimal_integer_constant(void);
void test_read_character_constant(void);
void test_read_octal_character_constant(void);
void test_read_hexadecimal_character_constant(void);
void test_read_string_literal(void);

void run_lexer_test(char* input, Vector* expected);

CU_Suite* add_test_suite_lexer(void) {
    CU_Suite* suite = CU_add_suite("test_suite_lexer", NULL, NULL);
    CU_ADD_TEST(suite, test_read_function_definition);
    CU_ADD_TEST(suite, test_read_declaration_integer_without_init);
    CU_ADD_TEST(suite, test_read_declaration_pointer_without_init);
    CU_ADD_TEST(suite, test_read_declaration_aggregate_without_init);
    CU_ADD_TEST(suite, test_read_declaration_function_without_init);
    CU_ADD_TEST(suite, test_read_declaration_int_with_init);
    CU_ADD_TEST(suite, test_read_declaration_long_with_init);
    CU_ADD_TEST(suite, test_read_declaration_long_long_with_init);
    CU_ADD_TEST(suite, test_read_declaration_pointer_with_init);
    CU_ADD_TEST(suite, test_read_declaration_aggregate_with_init);
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
    CU_ADD_TEST(suite, test_read_relational_expr);
    CU_ADD_TEST(suite, test_read_additive_expr);
    CU_ADD_TEST(suite, test_read_multiplicative_expr);
    CU_ADD_TEST(suite, test_read_unary_expr);
    CU_ADD_TEST(suite, test_read_postfix_expr);
    return suite;
}

void test_read_function_definition(void) {
    char* input = "int test_read_function_definition(void) {\n"
                  "    printf(\"OK\\n\");"
                  "    return 0;"
                  "}";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("test_read_function_definition")));
    vector_push(expected, new_ctoken(CTOKEN_LPALEN));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_VOID));
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

void test_read_declaration_integer_without_init(void) {
    char* input = "char c; int i; unsigned u; long l; long long ll;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("u")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("l")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("ll")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_pointer_without_init(void) {
    char* input = "int* p_t; char* s; int a[5];";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("p_t")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("s")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACKET));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(expected, new_ctoken(CTOKEN_RBRACKET));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_aggregate_without_init(void) {
    char* input = "typedef struct X { int m; } X; typedef enum Y { PLUS, MINUS } Y;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("X")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("m")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("X")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("Y")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("PLUS")));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("MINUS")));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("Y")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_function_without_init(void) {
    char* input = "void put_int(int);";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("put_int")));
    vector_push(expected, new_ctoken(CTOKEN_LPALEN));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_ctoken(CTOKEN_RPALEN));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_int_with_init(void) {
    char* input = "char c = 'a'; int i = 0; unsigned u1 = 1u, u2 = 2U;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 'a')));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("u1")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 1u)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("u2")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 2U)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_long_with_init(void) {
    char* input = "long l1 = 2l, l2 = 4L; long unsigned lu1 = 1lu, lu2 = 2LU;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("l1")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONG, 2l)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("l2")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONG, 4L)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("lu1")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 1lu)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("lu2")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 2LU)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_long_long_with_init(void) {
    char* input = "long long ll1 = 3ll, ll2 = 6LL; long unsigned long lul1 = 1ull, lul2 = 8ULL;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("ll1")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONGLONG, 3ll)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("ll2")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONGLONG, 6LL)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("lul1")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 1ull)));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("lul2")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 8ULL)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_declaration_pointer_with_init(void) {
    char* input = "char* s = \"Hello\"; int a[3] = {8, 2, 0};";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(expected, new_ctoken(CTOKEN_ASTERISK));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("s")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("Hello"), 6)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
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

void test_read_declaration_aggregate_with_init(void) {
    char* input = "struct X { int m; } x = {3}; enum Y { PLUS, MINUS } y = PLUS;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("X")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("m")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("Y")));
    vector_push(expected, new_ctoken(CTOKEN_LBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("PLUS")));
    vector_push(expected, new_ctoken(CTOKEN_COMMA));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("MINUS")));
    vector_push(expected, new_ctoken(CTOKEN_RBRACE));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("PLUS")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_assignment_expr(void) {
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

void test_read_logical_expr(void) {
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

void test_read_equality_expr(void) {
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

void test_read_relational_expr(void) {
    char* input = "a < 0; b > 1; c <= 2; d >= 3;";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(expected, new_ctoken(CTOKEN_LESS));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(expected, new_ctoken(CTOKEN_GREATER));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(expected, new_ctoken(CTOKEN_LESS_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("d")));
    vector_push(expected, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_additive_expr(void) {
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

void test_read_multiplicative_expr(void) {
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

void test_read_unary_expr(void) {
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

void test_read_postfix_expr(void) {
    char* input = "arr[10][4]; exec_process(a,  b); s.x = t->y;";

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
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("s")));
    vector_push(expected, new_ctoken(CTOKEN_DOT));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(expected, new_ctoken(CTOKEN_EQUAL));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("t")));
    vector_push(expected, new_ctoken(CTOKEN_ARROW));
    vector_push(expected, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(expected, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_decimal_integer_constant(void) {
    char* input =
        "1 2147483647 1u 4294967295U 1L 9223372036854775807l"
        "1ul 2lu 3uL 4Lu 18446744073709551612Ul 18446744073709551613lU 18446744073709551614UL 18446744073709551615LU\n"
        "1ull 2llu 3uLL 4LLu 18446744073709551612Ull 18446744073709551613llU 18446744073709551614ULL "
        "18446744073709551615LLU";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2147483647)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 1u)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 4294967295U)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONG, 1L)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONG, 9223372036854775807l)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 1ul)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 2lu)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 3uL)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 4Lu)));
    vector_push(expected,
                new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 18446744073709551612Ul)));
    vector_push(expected,
                new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 18446744073709551613lU)));
    vector_push(expected,
                new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 18446744073709551614UL)));
    vector_push(expected,
                new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 18446744073709551615LU)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 1ull)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 2llu)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 3uLL)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 4LLu)));
    vector_push(expected, new_iliteral_ctoken(
                              CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 18446744073709551612Ull)));
    vector_push(expected, new_iliteral_ctoken(
                              CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 18446744073709551613llU)));
    vector_push(expected, new_iliteral_ctoken(
                              CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 18446744073709551614ULL)));
    vector_push(expected, new_iliteral_ctoken(
                              CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 18446744073709551615LLU)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_octal_integer_constant(void) {
    char* input = "04 076 0123 05 0 017777777777";

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

void test_read_hexadecimal_integer_constant(void) {
    char* input = "0x12 0X034 0XaFd 0X6cB 0x7e 0x598 0X0 0x7FFFFFFF";

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

void test_read_character_constant(void) {
    char* input = "'0' 'a' 'xy' '#' '\\a' '\\b' '\\f' '\\r' '\\t' '\\v' '\\\"' '\\'' '\\?' '\\\\' '\\r\\n'";

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

void test_read_octal_character_constant(void) {
    char* input = "'\\0' '\\13' '\\107'";

    Vector* expected = new_vector(&t_ctoken);
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 013)));
    vector_push(expected, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 0107)));
    vector_push(expected, new_ctoken(CTOKEN_EOF));

    run_lexer_test(input, expected);

    delete_vector(expected);
}

void test_read_hexadecimal_character_constant(void) {
    char* input = "'\\x12' '\\x034' '\\xaF' '\\xcB' '\\x7e' '\\x59' '\\xff' '\\x0'";

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

void test_read_string_literal(void) {
    char* input = "\"\" \"str literal\" \"str\\tliteral\"";

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

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_lexer(lexer);

    fclose(file_ptr);
}
