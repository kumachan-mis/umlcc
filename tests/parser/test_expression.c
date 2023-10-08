#include "./test_expression.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/expression.h"
#include "../testlib/testlib.h"

void test_parse_assignment_expr(void);
void test_parse_multiply_assignment_expr(void);
void test_parse_division_assignment_expr(void);
void test_parse_modulo_assignment_expr(void);
void test_parse_add_assignment_expr(void);
void test_parse_subtract_assignment_expr(void);
void test_parse_bitwise_inclusive_or_assignment_expr(void);
void test_parse_bitwise_exclusive_or_assignment_expr(void);
void test_parse_bitwise_and_assignment_expr(void);
void test_parse_conditional_expr(void);
void test_parse_logical_or_expr(void);
void test_parse_logical_and_expr(void);
void test_parse_bitwise_inclusive_or_expr(void);
void test_parse_bitwise_exclusive_or_expr(void);
void test_parse_bitwise_and_expr(void);
void test_parse_equal_expr(void);
void test_parse_not_equal_expr(void);
void test_parse_less_expr(void);
void test_parse_greater_expr(void);
void test_parse_less_equal_expr(void);
void test_parse_greater_equal_expr(void);
void test_parse_add_expr(void);
void test_parse_subtract_expr(void);
void test_parse_multiply_expr(void);
void test_parse_division_expr(void);
void test_parse_modulo_expr(void);
void test_parse_cast_expr(void);
void test_parse_preinc_expr(void);
void test_parse_predec_expr(void);
void test_parse_address_expr(void);
void test_parse_indirection_expr(void);
void test_parse_positive_expr(void);
void test_parse_negative_expr(void);
void test_parse_bitwise_not_expr(void);
void test_parse_logical_not_expr(void);
void test_parse_sizeof_expr_typename(void);
void test_parse_sizeof_expr_expr(void);
void test_parse_call_expr(void);
void test_parse_subscription_expr(void);
void test_parse_member_expr(void);
void test_parse_tomember_expr(void);
void test_parse_postinc_expr(void);
void test_parse_postdec_expr(void);
void test_parse_ident_expr(void);
void test_parse_iliteral_expr_char(void);
void test_parse_iliteral_expr_int(void);
void test_parse_sliteral_expr(void);
void test_parse_parenthesized_expr(void);

void run_expr_parser_test(Vector* input, Ast* expected);

CU_Suite* add_test_suite_expr_parser(void) {
    CU_Suite* suite = CU_add_suite("test_suite_expr_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_assignment_expr);
    CU_ADD_TEST(suite, test_parse_multiply_assignment_expr);
    CU_ADD_TEST(suite, test_parse_division_assignment_expr);
    CU_ADD_TEST(suite, test_parse_modulo_assignment_expr);
    CU_ADD_TEST(suite, test_parse_add_assignment_expr);
    CU_ADD_TEST(suite, test_parse_subtract_assignment_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_inclusive_or_assignment_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_exclusive_or_assignment_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_and_assignment_expr);
    CU_ADD_TEST(suite, test_parse_conditional_expr);
    CU_ADD_TEST(suite, test_parse_logical_or_expr);
    CU_ADD_TEST(suite, test_parse_logical_and_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_inclusive_or_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_exclusive_or_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_and_expr);
    CU_ADD_TEST(suite, test_parse_equal_expr);
    CU_ADD_TEST(suite, test_parse_not_equal_expr);
    CU_ADD_TEST(suite, test_parse_less_expr);
    CU_ADD_TEST(suite, test_parse_greater_expr);
    CU_ADD_TEST(suite, test_parse_less_equal_expr);
    CU_ADD_TEST(suite, test_parse_greater_equal_expr);
    CU_ADD_TEST(suite, test_parse_add_expr);
    CU_ADD_TEST(suite, test_parse_subtract_expr);
    CU_ADD_TEST(suite, test_parse_multiply_expr);
    CU_ADD_TEST(suite, test_parse_division_expr);
    CU_ADD_TEST(suite, test_parse_modulo_expr);
    CU_ADD_TEST(suite, test_parse_cast_expr);
    CU_ADD_TEST(suite, test_parse_preinc_expr);
    CU_ADD_TEST(suite, test_parse_predec_expr);
    CU_ADD_TEST(suite, test_parse_address_expr);
    CU_ADD_TEST(suite, test_parse_indirection_expr);
    CU_ADD_TEST(suite, test_parse_positive_expr);
    CU_ADD_TEST(suite, test_parse_negative_expr);
    CU_ADD_TEST(suite, test_parse_bitwise_not_expr);
    CU_ADD_TEST(suite, test_parse_logical_not_expr);
    CU_ADD_TEST(suite, test_parse_sizeof_expr_typename);
    CU_ADD_TEST(suite, test_parse_sizeof_expr_expr);
    CU_ADD_TEST(suite, test_parse_call_expr);
    CU_ADD_TEST(suite, test_parse_subscription_expr);
    CU_ADD_TEST(suite, test_parse_member_expr);
    CU_ADD_TEST(suite, test_parse_tomember_expr);
    CU_ADD_TEST(suite, test_parse_postinc_expr);
    CU_ADD_TEST(suite, test_parse_postdec_expr);
    CU_ADD_TEST(suite, test_parse_ident_expr);
    CU_ADD_TEST(suite, test_parse_iliteral_expr_char);
    CU_ADD_TEST(suite, test_parse_iliteral_expr_int);
    CU_ADD_TEST(suite, test_parse_sliteral_expr);
    CU_ADD_TEST(suite, test_parse_parenthesized_expr);
    return suite;
}

void test_parse_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("q")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    // assignment operators have right-to-left associativity.
    Ast* expected = new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                            new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                            new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_ast(AST_COND_EXPR, 3, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("q")))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_multiply_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_MUL_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_MUL_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_division_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SLASH_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SLASH_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DIV_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_DIV_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_modulo_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PERCENT_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_PERCENT_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_MOD_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_MOD_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_add_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_PLUS_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_ADD_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_ADD_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_subtract_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_MINUS_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_SUB_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_SUB_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_inclusive_or_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_VBAR_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_VBAR_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_OR_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_OR_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_exclusive_or_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_CARET_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_CARET_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_XOR_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_XOR_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_and_assignment_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_AND_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_AND_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_AND_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_AND_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_conditional_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_QUESTION));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COLON));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_COND_EXPR, 3,            // non-terminal
                            new_ast(AST_GREATER_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_COND_EXPR, 3,         // non-terminal
                                    new_ast(AST_LESS_EXPR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                                    new_ast(AST_NEG_EXPR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_logical_or_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("m")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("n")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_LOR_EXPR, 2,                           // non-terminal
                new_ast(AST_LOR_EXPR, 2,                   // non-terminal
                        new_ast(AST_LOR_EXPR, 2,           // non-terminal
                                new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                                new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                        new_ast(AST_SUB_EXPR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("m")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                        new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)))),
                new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                        new_ast(AST_SUB_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("n")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_logical_and_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("check1")));
    vector_push(input, new_ctoken(CTOKEN_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("check2")));
    vector_push(input, new_ctoken(CTOKEN_AND_AND));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("validate")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("object")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LAND_EXPR, 2,       // non-terminal
                            new_ast(AST_OR_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("check1")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("check2"))),
                            new_ast(AST_CALL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("validate")),
                                    new_ast(AST_ARG_LIST, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("object")))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_inclusive_or_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_CARET));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_OR_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_XOR_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_exclusive_or_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_CARET));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_XOR_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_AND_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("z"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_and_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_AND_EXPR, 2,           // non-terminal
                            new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_equal_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_EQUAL_EXPR, 2,             // non-terminal
                            new_ast(AST_GREATEREQ_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                            new_ast(AST_LESS_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 8))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_not_equal_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("zero_flag")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_NEQUAL_EXPR, 2,        // non-terminal
                            new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("zero_flag")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_less_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("m")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LESS_EXPR, 2,        // non-terminal
                            new_ast(AST_ADD_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("m")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_greater_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_GREATER));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("n")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_GREATER_EXPR, 2,     // non-terminal
                            new_ast(AST_SUB_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("n")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_less_equal_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_LESS_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_GREATER));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_GREATER_EXPR, 2,        // non-terminal
                            new_ast(AST_LESSEQ_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("j"))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_greater_equal_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_GREATER_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LESS_EXPR, 2,              // non-terminal
                            new_ast(AST_GREATEREQ_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("i"))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_add_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("n")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_ADD_EXPR, 2, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                            new_ast(AST_MUL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("n")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_subtract_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 4)));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 6)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_SUB_EXPR, 2,         // non-terminal
                            new_ast(AST_ADD_EXPR, 2, // non-terminal
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_multiply_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 4)));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("integer_ref")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_MUL_EXPR, 2, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                            new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("integer_ref"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_division_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("amount")));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DIV_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("amount")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_modulo_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("value")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 9)));
    vector_push(input, new_ctoken(CTOKEN_PERCENT));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_MOD_EXPR, 2,         // non-terminal
                            new_ast(AST_MUL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("value")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_cast_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                            new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                    new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                            new_ast(AST_TYPE_VOID, 0)),
                                    new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                            new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                    new_ast(AST_ABS_DECLOR, 0),
                                                    new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                            new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                    new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                            new_ast(AST_TYPE_VOID, 0)),
                                                                    new_ast(AST_ABS_DECLOR, 0)))))),
                            new_ast(AST_CAST_EXPR, 2,                      // non-terminal
                                    new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                            new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_ast(AST_ABS_DECLOR, 0)),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("f"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_preinc_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_PLUS_PLUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_PREINC_EXPR, 1,        // non-terminal
                            new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("i"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_predec_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_MINUS_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_PREDEC_EXPR, 1, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("j")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_address_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_ADDR_EXPR, 1,          // non-terminal
                            new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_indirection_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_positive_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_POS_EXPR, 1, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_negative_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_ADD_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_ast(AST_NEG_EXPR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_bitwise_not_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_TILDE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("bits")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_NOT_EXPR, 1, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("bits")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_logical_not_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("flag")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LNOT_EXPR, 1,         // non-terminal
                            new_ast(AST_LNOT_EXPR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("flag"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_sizeof_expr_typename(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_SIZEOF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                            new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                    new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                            new_ast(AST_TYPE_INT, 0)),
                                    new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                            new_ast(AST_ABS_DECLOR, 0))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_sizeof_expr_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_SIZEOF));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_SIZEOF_EXPR, 1, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_call_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("function")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_CALL_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                            new_ast(AST_ARG_LIST, 2,            // non-terminal
                                    new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 7))),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_subscription_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("array")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_SUBSC_EXPR, 2,         // non-terminal
                            new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("array")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_member_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("structure")));
    vector_push(input, new_ctoken(CTOKEN_DOT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("child")));
    vector_push(input, new_ctoken(CTOKEN_DOT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("grandchild")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_MEMBER_EXPR, 2,         // non-terminal
                            new_ast(AST_MEMBER_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("structure")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("child"))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("grandchild")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_tomember_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("node")));
    vector_push(input, new_ctoken(CTOKEN_ARROW));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("next")));
    vector_push(input, new_ctoken(CTOKEN_ARROW));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("next")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_TOMEMBER_EXPR, 2,         // non-terminal
                            new_ast(AST_TOMEMBER_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("node")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("next"))),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("next")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_postinc_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_PLUS_PLUS));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_POSTINC_EXPR, 1, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("i")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_postdec_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("j")));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_MINUS_MINUS));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_POSTDEC_EXPR, 1,       // non-terminal
                            new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("b")),
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("j"))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_ident_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("variable")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_identifier_ast(AST_IDENT_EXPR, new_string("variable"));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_iliteral_expr_char(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 89)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_iliteral_expr_int(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_sliteral_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("Hello"), 6)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_sliteral_ast(AST_STRING_EXPR, new_sliteral(new_string("Hello"), 6));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_parenthesized_expr(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 88)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LNOT_EXPR, 1,           // non-terminal
                            new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                                    new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void run_expr_parser_test(Vector* input, Ast* expected) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_expr(parser));

    testlib_assert_ast_equal(actual, expected);
    CU_ASSERT_PTR_NULL(err);

    if (actual != NULL) {
        delete_ast(actual);
    }
    delete_parser(parser);
}
