#include "./test_statement.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/statement.h"
#include "../testlib/testlib.h"

void test_parse_compound_stmt_complete_typedef(void);
void test_parse_compound_stmt_incomplete_typedef(void);
void test_parse_compound_stmt_int(void);
void test_parse_compound_stmt_unsigned_int(void);
void test_parse_compound_stmt_long(void);
void test_parse_compound_stmt_unsigned_long(void);
void test_parse_compound_stmt_long_long(void);
void test_parse_compound_stmt_unsigned_long_long(void);
void test_parse_compound_stmt_void_pointer(void);
void test_parse_compound_stmt_empty(void);
void test_parse_return_stmt_non_void(void);
void test_parse_return_stmt_void(void);
void test_parse_expression_stmt(void);
void test_parse_null_stmt(void);
void test_parse_if_stmt(void);
void test_parse_if_else_stmt(void);
void test_parse_if_else_stmt_chain(void);
void test_parse_while_stmt(void);

void run_stmt_parser_test(Vector* input, Ast* expected);

CU_Suite* add_test_suite_stmt_parser(void) {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_compound_stmt_complete_typedef);
    CU_ADD_TEST(suite, test_parse_compound_stmt_incomplete_typedef);
    CU_ADD_TEST(suite, test_parse_compound_stmt_int);
    CU_ADD_TEST(suite, test_parse_compound_stmt_unsigned_int);
    CU_ADD_TEST(suite, test_parse_compound_stmt_long);
    CU_ADD_TEST(suite, test_parse_compound_stmt_unsigned_long);
    CU_ADD_TEST(suite, test_parse_compound_stmt_long_long);
    CU_ADD_TEST(suite, test_parse_compound_stmt_unsigned_long_long);
    CU_ADD_TEST(suite, test_parse_compound_stmt_void_pointer);
    CU_ADD_TEST(suite, test_parse_compound_stmt_empty);
    CU_ADD_TEST(suite, test_parse_return_stmt_non_void);
    CU_ADD_TEST(suite, test_parse_return_stmt_void);
    CU_ADD_TEST(suite, test_parse_expression_stmt);
    CU_ADD_TEST(suite, test_parse_null_stmt);
    CU_ADD_TEST(suite, test_parse_if_stmt);
    CU_ADD_TEST(suite, test_parse_if_else_stmt);
    CU_ADD_TEST(suite, test_parse_if_else_stmt_chain);
    CU_ADD_TEST(suite, test_parse_while_stmt);
    return suite;
}

void test_parse_compound_stmt_complete_typedef(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("pint")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("pint")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("q")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("q")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_CMPD_STMT, 3,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("pint")))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_identifier_ast(AST_TYPEDEF_NAME, new_string("pint"))),
                        new_ast(AST_INIT_DECLOR_LIST, 2,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))),
                                new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("q"))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("p"))),
                                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("q"))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 7))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_incomplete_typedef(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("next")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("s")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("malloc")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_SIZEOF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 3,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_STG_TYPEDEF, 0),
                        new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("Struct"))))),
        new_ast(
            AST_DECL, 1, // non-terminal
            new_ast(
                AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                        new_ast(AST_STRUCT_DECL_LIST, 2,               // non-terminal
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")))),
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_identifier_ast(AST_TYPEDEF_NAME, new_string("Struct"))),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("next"))))))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_identifier_ast(AST_TYPEDEF_NAME, new_string("Struct"))),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("s"))),
                                new_ast(AST_CALL_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("malloc")),
                                        new_ast(AST_ARG_LIST, 1,                               // non-terminal
                                                new_ast(AST_SIZEOF_EXPR, 1,                    // non-terminal
                                                        new_ast(AST_TYPE_NAME, 2,              // non-terminal
                                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                                        new_identifier_ast(AST_TYPEDEF_NAME,
                                                                                           new_string("Struct"))),
                                                                new_ast(AST_ABS_DECLOR, 0)))))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_int(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_CMPD_STMT, 3,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 3,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                                new_ast(AST_SUB_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))),
                new_ast(AST_EXPR_STMT, 1,           // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_ast(AST_MUL_EXPR, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_unsigned_int(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 4u)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 2u)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SLASH));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 4u))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_UNSIGNED, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 2u))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                        new_ast(AST_DIV_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_long(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONG, 5l)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONG, 3l)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_CMPD_STMT, 4,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONG, 5l))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONG, 3l))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                                new_ast(AST_ADD_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_unsigned_long(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 6ul)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 4ul)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 2, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 6ul))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 3, // non-terminal
                        new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 4ul))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 3, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_LONG, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                        new_ast(AST_SUB_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_long_long(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONGLONG, 7ll)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_LONGLONG, 5ll)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_CMPD_STMT, 4,                  // non-terminal
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONGLONG, 7ll))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 3, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_LONGLONG, 5ll))))),
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 3, // non-terminal
                                new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_LONG, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
                new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                                new_ast(AST_ADD_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_unsigned_long_long(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 8ull)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 6ull)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("z")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 4, // non-terminal
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 3, // non-terminal
                    new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                    new_ast(AST_INIT_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                            new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 8ull))))),
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 4, // non-terminal
                    new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0),
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                    new_ast(AST_INIT_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("y")),
                            new_iliteral_ast(AST_INT_EXPR, new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 6ull))))),
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 4, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0),
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))),
        new_ast(AST_EXPR_STMT, 1,                  // non-terminal
                new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                        new_ast(AST_INDIR_EXPR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("z"))),
                        new_ast(AST_MUL_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y"))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_void_pointer(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("malloc")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("n")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("free")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("malloc")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 4)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("free")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 4,                  // non-terminal
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_VOID, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                   // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("malloc"))),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("n")))))))),
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 1, // non-terminal
                    new_ast(AST_TYPE_VOID, 0)),
            new_ast(
                AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("free")),
                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_VOID, 0)),
                                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))))))))),
        new_ast(
            AST_DECL, 2,               // non-terminal
            new_ast(AST_DECL_SPECS, 1, // non-terminal
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                    new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                            new_ast(AST_CALL_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("malloc")),
                                    new_ast(AST_ARG_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))))))),
        new_ast(AST_EXPR_STMT, 1,
                new_ast(AST_CALL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("free")),
                        new_ast(AST_ARG_LIST, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")))))

    );

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_compound_stmt_empty(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_CMPD_STMT, 0);

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_return_stmt_non_void(void) {
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

void test_parse_return_stmt_void(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_RET_STMT, 0);

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_expression_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 12)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_EXPR_STMT, 1,           // non-terminal
                            new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 12))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_null_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_NULL_STMT, 0);

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_if_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_CMPD_STMT, 2,                  // non-terminal
                new_ast(AST_IF_STMT, 2,            // non-terminal
                        new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                new_ast(AST_EXPR_STMT, 1,           // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_if_else_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ELSE));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_CMPD_STMT, 1,                  // non-terminal
        new_ast(AST_IF_STMT, 3,            // non-terminal
                new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                new_ast(AST_CMPD_STMT, 1,                   // non-terminal
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                new_ast(AST_CMPD_STMT, 1,                   // non-terminal
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_if_else_stmt_chain(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ELSE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_IF));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_GREATER));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ELSE));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_IF_STMT, 3,           // non-terminal
        new_ast(AST_LESS_EXPR, 2, // non-terminal
                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
        new_ast(AST_CMPD_STMT, 1,                   // non-terminal
                new_ast(AST_EXPR_STMT, 1,           // non-terminal
                        new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                new_ast(AST_NEG_EXPR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")))))),

        new_ast(AST_IF_STMT, 3,              // non-terminal
                new_ast(AST_GREATER_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                new_ast(AST_CMPD_STMT, 1,                   // non-terminal
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x"))))),
                new_ast(AST_CMPD_STMT, 1,                   // non-terminal
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void test_parse_while_stmt(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_WHILE));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LESS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 10)));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_PLUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_WHILE_STMT, 2,        // non-terminal
                new_ast(AST_LESS_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 10))),
                new_ast(AST_CMPD_STMT, 2,                   // non-terminal
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_ast(AST_ADD_EXPR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                        new_ast(AST_EXPR_STMT, 1,           // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                                        new_ast(AST_MUL_EXPR, 2, // non-terminal
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("y")))))));

    run_stmt_parser_test(input, expected);

    delete_ast(expected);
}

void run_stmt_parser_test(Vector* input, Ast* expected) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_stmt(parser));

    testlib_assert_ast_equal(actual, expected);
    CU_ASSERT_PTR_NULL(err);

    if (actual != NULL) {
        delete_ast(actual);
    }
    delete_parser(parser);
}
