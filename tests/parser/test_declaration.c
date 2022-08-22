#include "./test_declaration.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/declaration.h"
#include "../testlib/testlib.h"

void test_parse_char_decl();
void test_parse_int_decl();
void test_parse_pointer_decl();
void test_parse_array_decl();
void test_parse_function_decl();
void test_parse_parameter_decl();
void test_parse_typedef_decl();
void test_parse_expr_init();
void test_parse_list_init_empty();
void test_parse_list_init_without_trailing_comma();
void test_parse_list_init_with_trailing_comma();

void run_decl_parser_test(Vector* __restrict__ input, Set* __restrict__ typedef_names_set,
                          Ast* __restrict__ expected);

CU_Suite* add_test_suite_decl_parser() {
    CU_Suite* suite = CU_add_suite("test_suite_decl_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_char_decl);
    CU_ADD_TEST(suite, test_parse_int_decl);
    CU_ADD_TEST(suite, test_parse_pointer_decl);
    CU_ADD_TEST(suite, test_parse_array_decl);
    CU_ADD_TEST(suite, test_parse_function_decl);
    CU_ADD_TEST(suite, test_parse_parameter_decl);
    CU_ADD_TEST(suite, test_parse_typedef_decl);
    CU_ADD_TEST(suite, test_parse_expr_init);
    CU_ADD_TEST(suite, test_parse_list_init_empty);
    CU_ADD_TEST(suite, test_parse_list_init_without_trailing_comma);
    CU_ADD_TEST(suite, test_parse_list_init_with_trailing_comma);
    return suite;
}

void test_parse_char_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("c")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_int_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("i")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_pointer_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("q")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 2,           // non-terminal
                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("p")))),
                new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                        new_ast(AST_PTR_DECLOR, 1,         // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("q")))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_array_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 6)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("d")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 4)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 4,             // non-terminal
            new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)))),
            new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2,       // non-terminal
                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("b"))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 8)))),
            new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                            new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("c")),
                                    new_iliteral_ast(AST_INT_EXPR,
                                                     new_signed_iliteral(INTEGER_INT, 3))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)))),
            new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                            new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                    new_ast(AST_PTR_DECLOR, 1,
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("d"))),
                                    new_iliteral_ast(AST_INT_EXPR,
                                                     new_signed_iliteral(INTEGER_INT, 4))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_function_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("g")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("h")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 3,                   // non-terminal
            new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                            new_ast(AST_PARAM_LIST, 0))),
            new_ast(
                AST_INIT_DECLOR, 1,         // non-terminal
                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("g")),
                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")))))),
            new_ast(
                AST_INIT_DECLOR, 1,         // non-terminal
                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("h")),
                        new_ast(AST_PARAM_LIST, 2,                      // non-terminal
                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("b"))),
                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("c"))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_parameter_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("func")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1, // non-terminal
            new_ast(
                AST_INIT_DECLOR, 1, // non-terminal
                new_ast(
                    AST_FUNC_DECLOR, 2, // non-terminal
                    new_identifier_ast(AST_IDENT_DECLOR, new_string("func")),
                    new_ast(
                        AST_PARAM_LIST, 2,                      // non-terminal
                        new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                        new_ast(AST_TYPE_INT, 0)),
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                        new_iliteral_ast(AST_INT_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 5)))),
                        new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                        new_ast(AST_TYPE_CHAR, 0)),
                                new_ast(
                                    AST_FUNC_DECLOR, 2,        // non-terminal
                                    new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                    new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                            new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                    new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                            new_ast(AST_TYPE_INT, 0)),
                                                    new_identifier_ast(AST_IDENT_DECLOR,
                                                                       new_string("p")))))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_typedef_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("test_type")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Set* typedef_names_set = new_set(&t_hashable_string);
    set_add(typedef_names_set, new_string("test_type"));

    Ast* expected =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_identifier_ast(AST_TYPEDEF_NAME, new_string("test_type"))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));

    run_decl_parser_test(input, typedef_names_set, expected);

    delete_ast(expected);
}

void test_parse_expr_init() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("scalar")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,    // non-terminal
            new_ast(AST_INIT_DECLOR, 2, // non-terminal
                    new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                    new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_list_init_empty() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("array")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,             // non-terminal
            new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                    new_ast(AST_INIT_LIST, 0))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_list_init_without_trailing_comma() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("array")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,             // non-terminal
            new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))),
                    new_ast(AST_INIT_LIST, 2, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_list_init_with_trailing_comma() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("array")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,             // non-terminal
            new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))),
                    new_ast(AST_INIT_LIST, 1, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void run_decl_parser_test(Vector* __restrict__ input, Set* __restrict__ typedef_names_set,
                          Ast* __restrict__ expected) {
    Parser* parser = new_parser(input);
    if (typedef_names_set != NULL) {
        delete_set(parser->typedef_names_set);
        parser->typedef_names_set = typedef_names_set;
    }

    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_decl(parser));

    CU_ASSERT_TRUE(testlib_ast_equals(actual, expected));
    CU_ASSERT_PTR_NULL(err);

    delete_ast(actual);
    delete_parser(parser);
}
