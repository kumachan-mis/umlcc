#include "./test_statement.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/statement.h"
#include "../testlib/testlib.h"

void test_parse_compound_stmt_vardef();
void test_parse_compound_stmt_typedef();
void test_parse_compound_stmt_empty();
void test_parse_return_stmt();
void test_parse_expression_stmt();

void run_stmt_parser_test(Vector* __restrict__ input, Ast* __restrict__ expected);

CU_Suite* add_test_suite_stmt_parser() {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_compound_stmt_vardef);
    CU_ADD_TEST(suite, test_parse_compound_stmt_typedef);
    CU_ADD_TEST(suite, test_parse_compound_stmt_empty);
    CU_ADD_TEST(suite, test_parse_return_stmt);
    CU_ADD_TEST(suite, test_parse_expression_stmt);
    return suite;
}

void test_parse_compound_stmt_typedef() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("pint")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("pint")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ptr")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 3, // non-terminal
        new_ast(
            AST_DECL, 2,                    // non-terminal
            new_ast(AST_DECL_SPECIFIERS, 2, // non-terminal
                    new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                    new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("pint")))))),
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_identifier_ast(AST_TYPEDEF_NAME, new_string("pint"))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("ptr"))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("ptr"))),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 7)))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_vardef() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 2, // non-terminal
        new_ast(
            AST_DECL, 2,                    // non-terminal
            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                    new_ast(AST_INIT_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))),
        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                        new_ast(AST_MUL_EXPR, 2, // non-terminal
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_empty() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_CMPD_STMT, 0);

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_return_stmt() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_RET_STMT, 1, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_expression_stmt() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 12)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 12))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void run_stmt_parser_test(Vector* __restrict__ input, Ast* __restrict__ expected) {
    Parser* parser = new_parser(input);
    Ast* actual = parse_stmt(parser);

    CU_ASSERT_TRUE(testlib_ast_equals(actual, expected));

    delete_ast(actual);
    delete_parser(parser);
}