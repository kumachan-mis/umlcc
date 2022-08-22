#include "./test_expression_error.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/expression.h"
#include "../testlib/testlib.h"

void test_parse_assignment_expr_error();
void test_parse_logical_or_expr_error();
void test_parse_logical_and_expr_error();
void test_parse_equal_expr_error();
void test_parse_not_equal_expr_error();
void test_parse_add_expr_error();
void test_parse_subtract_expr_error();
void test_parse_multiply_expr_error();
void test_parse_division_expr_error();
void test_parse_modulo_expr_error();
void test_parse_address_expr_error();
void test_parse_indirection_expr_error();
void test_parse_logical_not_expr_error();
void test_parse_call_expr_arg_expr_error();
void test_parse_call_expr_arg_list_error();
void test_parse_subscription_expr_index_expr_error();
void test_parse_subscription_expr_bracket_error();
void test_parse_parenthesized_expr_inner_expr_error();
void test_parse_parenthesized_expr_paren_error();

void run_expr_parser_error_test(Vector* __restrict__ input, const char* __restrict__ message);

CU_Suite* add_test_suite_expr_parser_error() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_assignment_expr_error);
    CU_ADD_TEST(suite, test_parse_logical_or_expr_error);
    CU_ADD_TEST(suite, test_parse_logical_and_expr_error);
    CU_ADD_TEST(suite, test_parse_equal_expr_error);
    CU_ADD_TEST(suite, test_parse_not_equal_expr_error);
    CU_ADD_TEST(suite, test_parse_add_expr_error);
    CU_ADD_TEST(suite, test_parse_subtract_expr_error);
    CU_ADD_TEST(suite, test_parse_multiply_expr_error);
    CU_ADD_TEST(suite, test_parse_division_expr_error);
    CU_ADD_TEST(suite, test_parse_modulo_expr_error);
    CU_ADD_TEST(suite, test_parse_address_expr_error);
    CU_ADD_TEST(suite, test_parse_indirection_expr_error);
    CU_ADD_TEST(suite, test_parse_logical_not_expr_error);
    CU_ADD_TEST(suite, test_parse_call_expr_arg_expr_error);
    CU_ADD_TEST(suite, test_parse_call_expr_arg_list_error);
    CU_ADD_TEST(suite, test_parse_subscription_expr_index_expr_error);
    CU_ADD_TEST(suite, test_parse_subscription_expr_bracket_error);
    CU_ADD_TEST(suite, test_parse_parenthesized_expr_inner_expr_error);
    CU_ADD_TEST(suite, test_parse_parenthesized_expr_paren_error);
    return suite;
}

void test_parse_assignment_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token =\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_logical_or_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_ctoken(CTOKEN_AND_AND));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token &&\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_logical_and_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_AND_AND));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token ||\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_equal_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token EOF\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_not_equal_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token =\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_add_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token ]\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_subtract_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token ;\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_multiply_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token EOF\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_division_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token /\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_modulo_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_PERCENT));
    vector_push(input, new_ctoken(CTOKEN_PERCENT));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token %\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_address_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token =\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_indirection_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token EOF\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_logical_not_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token EOF\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_call_expr_arg_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token ,\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_call_expr_arg_list_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token , expected, but got !\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_subscription_expr_index_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token =\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_subscription_expr_bracket_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token ] expected, but got }\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_parenthesized_expr_inner_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token ;\n";
    run_expr_parser_error_test(input, message);
}

void test_parse_parenthesized_expr_paren_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token ) expected, but got EOF\n";
    run_expr_parser_error_test(input, message);
}

void run_expr_parser_error_test(Vector* __restrict__ input, const char* __restrict__ message) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_expr(parser));

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_parser(parser);
}
