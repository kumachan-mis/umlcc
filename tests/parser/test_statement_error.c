#include "./test_statement_error.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/statement.h"
#include "../testlib/testlib.h"

void test_parse_compound_stmt_inner_stmt_error();
void test_parse_compound_stmt_brace_error();
void test_parse_return_stmt_error();
void test_parse_expression_stmt_error();

void run_stmt_parser_error_test(Vector* __restrict__ input, const char* message);

CU_Suite* add_test_suite_stmt_parser_error() {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_compound_stmt_inner_stmt_error);
    CU_ADD_TEST(suite, test_parse_compound_stmt_brace_error);
    CU_ADD_TEST(suite, test_parse_return_stmt_error);
    CU_ADD_TEST(suite, test_parse_expression_stmt_error);
    return suite;
}

void test_parse_compound_stmt_inner_stmt_error() {
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

    const char* message = "Error: token ; expected, but got integer-constant\n";
    run_stmt_parser_error_test(input, message);
}

void test_parse_compound_stmt_brace_error() {
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

    const char* message = "Error: unexpected token EOF\n";
    run_stmt_parser_error_test(input, message);
}

void test_parse_return_stmt_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ret")));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token ; expected, but got integer-constant\n";
    run_stmt_parser_error_test(input, message);
}

void test_parse_expression_stmt_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token ; expected, but got identifier\n";
    run_stmt_parser_error_test(input, message);
}

void run_stmt_parser_error_test(Vector* __restrict__ input, const char* message) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_stmt(parser));

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_parser(parser);
}
