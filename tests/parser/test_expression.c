#include "./test_expression.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/expression.h"
#include "../testlib/testlib.h"

void test_parse_assignment_expr();
void test_parse_logical_or_expr();
void test_parse_logical_and_expr();
void test_parse_equal_expr();
void test_parse_not_equal_expr();
void test_parse_add_expr();
void test_parse_subtract_expr();
void test_parse_multiply_expr();
void test_parse_division_expr();
void test_parse_modulo_expr();
void test_parse_address_expr();
void test_parse_indirection_expr();
void test_parse_logical_not_expr();
void test_parse_call_expr();
void test_parse_subscription_expr();
void test_parse_ident_expr();
void test_parse_iliteral_expr_int();
void test_parse_iliteral_expr_char();
void test_parse_sliteral_expr();
void test_parse_parenthesized_expr();

void run_expr_parser_test(Vector* __restrict__ input, Ast* __restrict__ expected);

CU_Suite* add_test_suite_expr_parser() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_assignment_expr);
    CU_ADD_TEST(suite, test_parse_logical_or_expr);
    CU_ADD_TEST(suite, test_parse_logical_and_expr);
    CU_ADD_TEST(suite, test_parse_equal_expr);
    CU_ADD_TEST(suite, test_parse_not_equal_expr);
    CU_ADD_TEST(suite, test_parse_add_expr);
    CU_ADD_TEST(suite, test_parse_subtract_expr);
    CU_ADD_TEST(suite, test_parse_multiply_expr);
    CU_ADD_TEST(suite, test_parse_division_expr);
    CU_ADD_TEST(suite, test_parse_modulo_expr);
    CU_ADD_TEST(suite, test_parse_address_expr);
    CU_ADD_TEST(suite, test_parse_indirection_expr);
    CU_ADD_TEST(suite, test_parse_logical_not_expr);
    CU_ADD_TEST(suite, test_parse_call_expr);
    CU_ADD_TEST(suite, test_parse_subscription_expr);
    CU_ADD_TEST(suite, test_parse_ident_expr);
    CU_ADD_TEST(suite, test_parse_iliteral_expr_int);
    CU_ADD_TEST(suite, test_parse_iliteral_expr_char);
    CU_ADD_TEST(suite, test_parse_sliteral_expr);
    CU_ADD_TEST(suite, test_parse_parenthesized_expr);
    return suite;
}

void test_parse_assignment_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_VBAR_VBAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("q")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    // assignment operators have right-to-left associativity.
    Ast* expected = new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                            new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                            new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_ast(AST_LOR_EXPR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("q")))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_logical_or_expr() {
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

void test_parse_logical_and_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("check")));
    vector_push(input, new_ctoken(CTOKEN_EXCLAM_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_AND_AND));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("validate")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("object")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LAND_EXPR, 2,           // non-terminal
                            new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("check")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                            new_ast(AST_CALL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("validate")),
                                    new_ast(AST_ARG_LIST, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_EXPR, new_string("object")))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_equal_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_EQUAL_EXPR, 2,       // non-terminal
                            new_ast(AST_SUB_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                            new_ast(AST_DIV_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 8))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_not_equal_expr() {
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

void test_parse_add_expr() {
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

void test_parse_subtract_expr() {
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

void test_parse_multiply_expr() {
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

void test_parse_division_expr() {
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

void test_parse_modulo_expr() {
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

void test_parse_address_expr() {
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

void test_parse_indirection_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_logical_not_expr() {
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

void test_parse_call_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("function")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
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

void test_parse_subscription_expr() {
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

void test_parse_ident_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("variable")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_identifier_ast(AST_IDENT_EXPR, new_string("variable"));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_iliteral_expr_int() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_iliteral_expr_char() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 89)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_sliteral_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(new_string("Hello"), 6)));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_sliteral_ast(AST_STRING_EXPR, new_sliteral(new_string("Hello"), 6));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_parenthesized_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_EXCLAM));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, 88)));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_LNOT_EXPR, 1,           // non-terminal
                            new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                                    new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88))));

    run_expr_parser_test(input, expected);

    delete_ast(expected);
}

void run_expr_parser_test(Vector* __restrict__ input, Ast* __restrict__ expected) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_expr(parser));

    CU_ASSERT_TRUE(testlib_ast_equals(actual, expected));
    CU_ASSERT_PTR_NULL(err);

    delete_ast(actual);
    delete_parser(parser);
}
