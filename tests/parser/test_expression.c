#include "./test_expression.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/expression.h"
#include "../testlib/testlib.h"

void test_parse_address_expr();
void test_parse_indirection_expr();
void test_parse_logical_not_expr();
void test_parse_call_expr();
void test_parse_subscription_expr();
void test_parse_ident_expr();
void test_parse_iliteral_expr_int();
void test_parse_iliteral_expr_char();
void test_parse_sliteral_expr();

void run_expr_parser_test(Vector* __restrict__ input, Ast* __restrict__ expected);

CU_Suite* add_test_suite_expr_parser() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_address_expr);
    CU_ADD_TEST(suite, test_parse_indirection_expr);
    CU_ADD_TEST(suite, test_parse_logical_not_expr);
    CU_ADD_TEST(suite, test_parse_call_expr);
    CU_ADD_TEST(suite, test_parse_subscription_expr);
    CU_ADD_TEST(suite, test_parse_ident_expr);
    CU_ADD_TEST(suite, test_parse_iliteral_expr_int);
    CU_ADD_TEST(suite, test_parse_iliteral_expr_char);
    CU_ADD_TEST(suite, test_parse_sliteral_expr);
    return suite;
}

void test_parse_address_expr() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_AND));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_ADDR_EXPR, 1,          // non-terminal
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

    Ast* expected = new_ast(
        AST_CALL_EXPR, 2, // non-terminal
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

    Ast* expected =
        new_ast(AST_SUBSC_EXPR, 2,         // non-terminal
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

void run_expr_parser_test(Vector* __restrict__ input, Ast* __restrict__ expected) {
    Parser* parser = new_parser(input);
    Ast* actual = parse_expr(parser);

    CU_ASSERT_TRUE(testlib_ast_equals(actual, expected));

    delete_ast(actual);
    delete_parser(parser);
}
