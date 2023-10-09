#include "./test_statement_error.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/statement.h"
#include "../testlib/testlib.h"

void test_parse_compound_stmt_error_child(void);
void test_parse_compound_stmt_error_braces(void);
void test_parse_return_stmt_error(void);
void test_parse_expression_stmt_error(void);
void test_parse_if_else_stmt_error_controlling_lparen(void);
void test_parse_if_else_stmt_error_controlling_expr(void);
void test_parse_if_else_stmt_error_controlling_rparen(void);
void test_parse_if_else_stmt_error_then_stmt(void);
void test_parse_if_else_stmt_error_else_stmt(void);
void test_parse_while_stmt_error_controlling_lparen(void);
void test_parse_while_stmt_error_controlling_expr(void);
void test_parse_while_stmt_error_controlling_rparen(void);
void test_parse_while_stmt_error_body(void);
void test_parse_for_stmt_error_lparen(void);
void test_parse_for_stmt_error_init_decl(void);
void test_parse_for_stmt_error_init_expr(void);
void test_parse_for_stmt_error_init_expr_semicolon(void);
void test_parse_for_stmt_error_controlling_expr(void);
void test_parse_for_stmt_error_controlling_expr_semicolon(void);
void test_parse_for_stmt_error_expr(void);
void test_parse_for_stmt_error_expr_rparen(void);
void test_parse_for_stmt_error_body(void);

void run_stmt_parser_error_test(Vector* input, Error* expected);

CU_Suite* add_test_suite_stmt_parser_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_compound_stmt_error_child);
    CU_ADD_TEST(suite, test_parse_compound_stmt_error_braces);
    CU_ADD_TEST(suite, test_parse_return_stmt_error);
    CU_ADD_TEST(suite, test_parse_expression_stmt_error);
    CU_ADD_TEST(suite, test_parse_if_else_stmt_error_controlling_lparen);
    CU_ADD_TEST(suite, test_parse_if_else_stmt_error_controlling_expr);
    CU_ADD_TEST(suite, test_parse_if_else_stmt_error_controlling_rparen);
    CU_ADD_TEST(suite, test_parse_if_else_stmt_error_then_stmt);
    CU_ADD_TEST(suite, test_parse_if_else_stmt_error_else_stmt);
    CU_ADD_TEST(suite, test_parse_while_stmt_error_controlling_lparen);
    CU_ADD_TEST(suite, test_parse_while_stmt_error_controlling_expr);
    CU_ADD_TEST(suite, test_parse_while_stmt_error_controlling_rparen);
    CU_ADD_TEST(suite, test_parse_while_stmt_error_body);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_lparen);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_init_decl);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_init_expr);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_init_expr_semicolon);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_controlling_expr);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_controlling_expr_semicolon);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_expr);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_expr_rparen);
    CU_ADD_TEST(suite, test_parse_for_stmt_error_body);
    return suite;
}

void test_parse_compound_stmt_error_child(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ; expected, but got integer-constant");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_compound_stmt_error_braces(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_return_stmt_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ret")));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ; expected, but got integer-constant");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_expression_stmt_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ; expected, but got identifier");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_if_else_stmt_error_controlling_lparen(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ( expected, but got identifier");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_if_else_stmt_error_controlling_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ==");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_if_else_stmt_error_controlling_rparen(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got {");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_if_else_stmt_error_then_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ELSE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token else");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_if_else_stmt_error_else_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ELSE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_while_stmt_error_controlling_lparen(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_WHILE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ( expected, but got identifier");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_while_stmt_error_controlling_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_WHILE));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ==");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_while_stmt_error_controlling_rparen(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_WHILE));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got {");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_while_stmt_error_body(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_WHILE));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_lparen(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ( expected, but got int");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_init_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ;");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_init_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ;");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_init_expr_semicolon(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ; expected, but got :");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_controlling_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ;");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_controlling_expr_semicolon(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 10)));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS_PLUS));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ; expected, but got identifier");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ;");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_expr_rparen(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 10)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_PLUS_PLUS));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got {");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_for_stmt_error_body(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_FOR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_stmt_parser_error_test(input, expected);

    delete_error(expected);
}

void run_stmt_parser_error_test(Vector* input, Error* expected) {
    Parser* parser = new_parser(input);
    Ast* ret = NULL;
    Error* actual = NULL;
    parserret_assign(&ret, &actual, parse_stmt(parser));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) {
        delete_error(actual);
    }
    delete_parser(parser);
}
