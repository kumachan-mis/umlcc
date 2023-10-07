#include "./test_expression_error.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/expression.h"
#include "../testlib/testlib.h"

void test_parse_assignment_expr_error(void);
void test_parse_conditional_expr_error_condition(void);
void test_parse_conditional_expr_error_lhs(void);
void test_parse_conditional_expr_error_colon(void);
void test_parse_conditional_expr_error_rhs(void);
void test_parse_logical_or_expr_error(void);
void test_parse_logical_and_expr_error(void);
void test_parse_bitwise_inclusive_or_expr_error(void);
void test_parse_bitwise_exclusive_or_expr_error(void);
void test_parse_bitwise_and_expr_error(void);
void test_parse_equal_expr_error(void);
void test_parse_not_equal_expr_error(void);
void test_parse_less_expr_error(void);
void test_parse_greater_expr_error(void);
void test_parse_less_equal_expr_error(void);
void test_parse_greater_equal_expr_error(void);
void test_parse_add_expr_error(void);
void test_parse_subtract_expr_error(void);
void test_parse_multiply_expr_error(void);
void test_parse_division_expr_error(void);
void test_parse_modulo_expr_error(void);
void test_parse_cast_expr_error_paren(void);
void test_parse_cast_expr_error_type_name(void);
void test_parse_preinc_expr_error(void);
void test_parse_predec_expr_error(void);
void test_parse_address_expr_error(void);
void test_parse_indirection_expr_error(void);
void test_parse_logical_not_expr_error(void);
void test_parse_positive_expr_error(void);
void test_parse_negative_expr_error(void);
void test_parse_bitwise_not_expr_error(void);
void test_parse_sizeof_expr_error_paren(void);
void test_parse_sizeof_expr_error_type_name(void);
void test_parse_call_expr_error_arg_expr(void);
void test_parse_call_expr_error_arg_list(void);
void test_parse_subscription_expr_error_index(void);
void test_parse_subscription_expr_error_bracket(void);
void test_parse_member_expr_error(void);
void test_parse_tomember_expr_error(void);
void test_parse_ident_expr_error_typedef_name(void);
void test_parse_parenthesized_expr_error_child(void);
void test_parse_parenthesized_expr_error_paren(void);

void run_expr_parser_error_test(Vector* input, Set* typedef_names_set, Error* expected);

CU_Suite* add_test_suite_expr_parser_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_expr_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_assignment_expr_error);
    CU_ADD_TEST(suite, test_parse_conditional_expr_error_condition);
    CU_ADD_TEST(suite, test_parse_conditional_expr_error_lhs);
    CU_ADD_TEST(suite, test_parse_conditional_expr_error_colon);
    CU_ADD_TEST(suite, test_parse_conditional_expr_error_rhs);
    CU_ADD_TEST(suite, test_parse_logical_or_expr_error);
    CU_ADD_TEST(suite, test_parse_logical_and_expr_error);
    CU_ADD_TEST(suite, test_parse_bitwise_inclusive_or_expr_error);
    CU_ADD_TEST(suite, test_parse_bitwise_exclusive_or_expr_error);
    CU_ADD_TEST(suite, test_parse_bitwise_and_expr_error);
    CU_ADD_TEST(suite, test_parse_equal_expr_error);
    CU_ADD_TEST(suite, test_parse_not_equal_expr_error);
    CU_ADD_TEST(suite, test_parse_less_expr_error);
    CU_ADD_TEST(suite, test_parse_greater_expr_error);
    CU_ADD_TEST(suite, test_parse_less_equal_expr_error);
    CU_ADD_TEST(suite, test_parse_greater_equal_expr_error);
    CU_ADD_TEST(suite, test_parse_add_expr_error);
    CU_ADD_TEST(suite, test_parse_subtract_expr_error);
    CU_ADD_TEST(suite, test_parse_multiply_expr_error);
    CU_ADD_TEST(suite, test_parse_division_expr_error);
    CU_ADD_TEST(suite, test_parse_modulo_expr_error);
    CU_ADD_TEST(suite, test_parse_cast_expr_error_paren);
    CU_ADD_TEST(suite, test_parse_cast_expr_error_type_name);
    CU_ADD_TEST(suite, test_parse_preinc_expr_error);
    CU_ADD_TEST(suite, test_parse_predec_expr_error);
    CU_ADD_TEST(suite, test_parse_address_expr_error);
    CU_ADD_TEST(suite, test_parse_indirection_expr_error);
    CU_ADD_TEST(suite, test_parse_logical_not_expr_error);
    CU_ADD_TEST(suite, test_parse_positive_expr_error);
    CU_ADD_TEST(suite, test_parse_negative_expr_error);
    CU_ADD_TEST(suite, test_parse_bitwise_not_expr_error);
    CU_ADD_TEST(suite, test_parse_sizeof_expr_error_paren);
    CU_ADD_TEST(suite, test_parse_sizeof_expr_error_type_name);
    CU_ADD_TEST(suite, test_parse_call_expr_error_arg_expr);
    CU_ADD_TEST(suite, test_parse_call_expr_error_arg_list);
    CU_ADD_TEST(suite, test_parse_subscription_expr_error_index);
    CU_ADD_TEST(suite, test_parse_subscription_expr_error_bracket);
    CU_ADD_TEST(suite, test_parse_member_expr_error);
    CU_ADD_TEST(suite, test_parse_tomember_expr_error);
    CU_ADD_TEST(suite, test_parse_ident_expr_error_typedef_name);
    CU_ADD_TEST(suite, test_parse_parenthesized_expr_error_child);
    CU_ADD_TEST(suite, test_parse_parenthesized_expr_error_paren);
    return suite;
}

void test_parse_assignment_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_conditional_expr_error_condition(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ?");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_conditional_expr_error_lhs(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token :");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_conditional_expr_error_colon(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token : expected, but got ;");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_conditional_expr_error_rhs(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_logical_or_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_ctoken(CTOKEN_AND_AND));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token &&");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_bitwise_inclusive_or_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_VBAR));
    vector_push(input, new_ctoken(CTOKEN_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token |");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_bitwise_exclusive_or_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_CARET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_bitwise_and_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_ctoken(CTOKEN_DOT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token .");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_logical_and_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_AND_AND));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ||");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_equal_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_not_equal_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_less_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_ctoken(CTOKEN_GREATER));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token >");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_greater_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token <");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_less_equal_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LESS_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token >=");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_greater_equal_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LESS_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token <=");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_add_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got ]");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_subtract_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got ;");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_multiply_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_division_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token /");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_modulo_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_PERCENT));
    vector_push(input, new_ctoken(CTOKEN_PERCENT));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token %%");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_cast_expr_error_paren(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got integer-constant");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_cast_expr_error_type_name(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("type")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("variable")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    // given "(int(*type))variable", parsing fails at "(int(*type", then goes back to "(int", expects ")", but got "("
    Error* expected = new_error("token ) expected, but got (");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_preinc_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_PLUS_PLUS));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_predec_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_MINUS_MINUS));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_address_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_indirection_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_positive_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_negative_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_bitwise_not_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_TILDE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_logical_not_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_sizeof_expr_error_paren(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_SIZEOF));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ( expected, but got *");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_sizeof_expr_error_type_name(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_SIZEOF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    // given "sizeof(int(*]", parsing fails at "sizeof(int(*]", then goes back to sizeof(int", expects ")", but got "("
    Error* expected = new_error("token ) expected, but got (");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_call_expr_error_arg_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ,");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_call_expr_error_arg_list(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got !");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_subscription_expr_error_index(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_subscription_expr_error_bracket(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ] expected, but got }");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_member_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_DOT));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token identifier expected, but got integer-constant");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_tomember_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_ARROW));
    vector_push(input, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("y"), 2)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token identifier expected, but got string-literal");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_ident_expr_error_typedef_name(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Type")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Set* typedef_names_set = new_set(&t_hashable_string);
    set_add(typedef_names_set, new_string("Type"));

    Error* expected = new_error("unexpected typedef name 'Type'");

    run_expr_parser_error_test(input, typedef_names_set, expected);

    delete_error(expected);
}

void test_parse_parenthesized_expr_error_child(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ;");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void test_parse_parenthesized_expr_error_paren(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ) expected, but got EOF");

    run_expr_parser_error_test(input, NULL, expected);

    delete_error(expected);
}

void run_expr_parser_error_test(Vector* input, Set* typedef_names_set, Error* expected) {
    Parser* parser = new_parser(input);
    if (typedef_names_set != NULL) {
        delete_set(parser->typedef_names_set);
        parser->typedef_names_set = typedef_names_set;
    }

    Ast* ret = NULL;
    Error* actual = NULL;
    parserret_assign(&ret, &actual, parse_expr(parser));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) {
        delete_error(actual);
    }
    delete_parser(parser);
}
