#include "./test_declaration.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/declaration.h"
#include "../testlib/testlib.h"

void test_parse_char_decl(void);
void test_parse_int_decl(void);
void test_parse_unsigned_int_decl(void);
void test_parse_long_decl(void);
void test_parse_unsigned_long_decl(void);
void test_parse_long_long_decl(void);
void test_parse_unsigned_long_long_decl(void);
void test_parse_pointer_decl(void);
void test_parse_array_decl(void);
void test_parse_function_decl(void);
void test_parse_non_scalar_parameter_decl(void);
void test_parse_unnamed_parameter_decl(void);
void test_parse_named_struct_decl(void);
void test_parse_unnamed_struct_decl(void);
void test_parse_nameonly_struct_decl(void);
void test_parse_struct_name_decl(void);
void test_parse_named_enum_decl(void);
void test_parse_named_enum_decl_comma(void);
void test_parse_unnamed_enum_decl(void);
void test_parse_nameonly_enum_decl(void);
void test_parse_enum_name_decl(void);
void test_parse_typedef_decl(void);
void test_parse_typedef_name_decl(void);
void test_parse_enclosed_decl_int(void);
void test_parse_enclosed_decl_pointer(void);
void test_parse_enclosed_decl_array(void);
void test_parse_enclosed_decl_function(void);
void test_parse_enclosed_decl_abstract_params(void);
void test_parse_expr_init(void);
void test_parse_list_init(void);
void test_parse_list_init_comma(void);

void run_decl_parser_test(Vector* input, Set* typedef_names_set, Ast* expected);

CU_Suite* add_test_suite_decl_parser(void) {
    CU_Suite* suite = CU_add_suite("test_suite_decl_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_char_decl);
    CU_ADD_TEST(suite, test_parse_int_decl);
    CU_ADD_TEST(suite, test_parse_unsigned_int_decl);
    CU_ADD_TEST(suite, test_parse_long_decl);
    CU_ADD_TEST(suite, test_parse_unsigned_long_decl);
    CU_ADD_TEST(suite, test_parse_long_long_decl);
    CU_ADD_TEST(suite, test_parse_unsigned_long_long_decl);
    CU_ADD_TEST(suite, test_parse_pointer_decl);
    CU_ADD_TEST(suite, test_parse_array_decl);
    CU_ADD_TEST(suite, test_parse_function_decl);
    CU_ADD_TEST(suite, test_parse_non_scalar_parameter_decl);
    CU_ADD_TEST(suite, test_parse_unnamed_parameter_decl);
    CU_ADD_TEST(suite, test_parse_named_struct_decl);
    CU_ADD_TEST(suite, test_parse_unnamed_struct_decl);
    CU_ADD_TEST(suite, test_parse_nameonly_struct_decl);
    CU_ADD_TEST(suite, test_parse_struct_name_decl);
    CU_ADD_TEST(suite, test_parse_named_enum_decl);
    CU_ADD_TEST(suite, test_parse_named_enum_decl_comma);
    CU_ADD_TEST(suite, test_parse_unnamed_enum_decl);
    CU_ADD_TEST(suite, test_parse_nameonly_enum_decl);
    CU_ADD_TEST(suite, test_parse_enum_name_decl);
    CU_ADD_TEST(suite, test_parse_typedef_decl);
    CU_ADD_TEST(suite, test_parse_typedef_name_decl);
    CU_ADD_TEST(suite, test_parse_enclosed_decl_int);
    CU_ADD_TEST(suite, test_parse_enclosed_decl_pointer);
    CU_ADD_TEST(suite, test_parse_enclosed_decl_array);
    CU_ADD_TEST(suite, test_parse_enclosed_decl_function);
    CU_ADD_TEST(suite, test_parse_enclosed_decl_abstract_params);
    CU_ADD_TEST(suite, test_parse_expr_init);
    CU_ADD_TEST(suite, test_parse_list_init);
    CU_ADD_TEST(suite, test_parse_list_init_comma);
    return suite;
}

void test_parse_char_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                    new_ast(AST_TYPE_CHAR, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("c")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_int_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                    new_ast(AST_TYPE_INT, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("i")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_unsigned_int_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("u")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 2, // non-terminal
                                    new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_INT, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("u")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_long_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("l")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                    new_ast(AST_TYPE_LONG, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("l")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_unsigned_long_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ul")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 2, // non-terminal
                                    new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("ul")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_long_long_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ll")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 2, // non-terminal
                                    new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("ll")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_unsigned_long_long_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_UNSIGNED));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_LONG));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("ull")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 3, // non-terminal
                        new_ast(AST_TYPE_UNSIGNED, 0), new_ast(AST_TYPE_LONG, 0), new_ast(AST_TYPE_LONG, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("ull")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_pointer_decl(void) {
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

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
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

void test_parse_array_decl(void) {
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
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
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
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)))),
            new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                            new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                    new_ast(AST_PTR_DECLOR, 1, new_identifier_ast(AST_IDENT_DECLOR, new_string("d"))),
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_function_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("g")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("h")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("b")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("c")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 3,                   // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_VOID, 0)),
                                                        new_ast(AST_ABS_DECLOR, 0))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("g")),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("h")),
                                        new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("b"))),
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("c"))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_non_scalar_parameter_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("func")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("func")),
                                new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                                        new_iliteral_ast(AST_INT_EXPR,
                                                                         new_signed_iliteral(INTEGER_INT, 5)))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                new_ast(AST_TYPE_INT, 0)),
                                                                        new_identifier_ast(AST_IDENT_DECLOR,
                                                                                           new_string("p")))))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_unnamed_parameter_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("g")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("h")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_VOID, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 3,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_VOID, 0)),
                                                new_ast(AST_ABS_DECLOR, 0))))),
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("g")),
                                new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_ast(AST_ABS_DECLOR, 0)),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                        new_ast(AST_ABS_DECLOR, 0),
                                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                new_ast(AST_TYPE_INT, 0)),
                                                                        new_ast(AST_ABS_DECLOR, 0)))))))),
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("h")),
                                new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_ABS_DECLOR, 0)))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_named_struct_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("s")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 10)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("next")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("structure")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,                        // non-terminal
        new_ast(AST_DECL_SPECS, 1,          // non-terminal
                new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                        new_ast(AST_STRUCT_DECL_LIST, 3,               // non-terminal
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y")))),
                                new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                new_ast(AST_TYPE_CHAR, 0)),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1,   // non-terminal
                                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("s")),
                                                        new_iliteral_ast(AST_INT_EXPR,
                                                                         new_signed_iliteral(INTEGER_INT, 10))))),
                                new_ast(AST_STRUCT_DECL, 2,                 // non-terminal
                                        new_ast(AST_SPEC_QUAL_LIST, 1,      // non-terminal
                                                new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                                        new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                                        new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("next")))))))),
        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("structure"))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_unnamed_struct_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("member")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("structure")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,                                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                                     // non-terminal
                        new_ast(AST_TYPE_STRUCT, 1,                            // non-terminal
                                new_ast(AST_STRUCT_DECL_LIST, 1,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("member"))))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("structure"))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_nameonly_struct_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("structure")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,                        // non-terminal
                            new_ast(AST_DECL_SPECS, 1,          // non-terminal
                                    new_ast(AST_TYPE_STRUCT, 1, // non-terminal
                                            new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")))),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("structure")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_struct_name_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_STRUCT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Struct")));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 1,                        // non-terminal
                new_ast(AST_DECL_SPECS, 1,          // non-terminal
                        new_ast(AST_TYPE_STRUCT, 2, // non-terminal
                                new_identifier_ast(AST_STRUCT_NAME, new_string("Struct")),
                                new_ast(AST_STRUCT_DECL_LIST, 1,               // non-terminal
                                        new_ast(AST_STRUCT_DECL, 2,            // non-terminal
                                                new_ast(AST_SPEC_QUAL_LIST, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_STRUCT_DECLOR_LIST, 1, // non-terminal
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_named_enum_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("PLUS")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("MINUS")));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("enumeration")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,                                   // non-terminal
                            new_ast(AST_DECL_SPECS, 1,                     // non-terminal
                                    new_ast(AST_TYPE_ENUM, 1,              // non-terminal
                                            new_ast(AST_ENUMOR_LIST, 2,    // non-terminal
                                                    new_ast(AST_ENUMOR, 1, // non-terminal
                                                            new_identifier_ast(AST_ENUM_CONST, new_string("PLUS"))),
                                                    new_ast(AST_ENUMOR, 1, // non-terminal
                                                            new_identifier_ast(AST_ENUM_CONST, new_string("MINUS")))))),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_named_enum_decl_comma(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("PLUS")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("MINUS")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("enumeration")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,                                   // non-terminal
                            new_ast(AST_DECL_SPECS, 1,                     // non-terminal
                                    new_ast(AST_TYPE_ENUM, 1,              // non-terminal
                                            new_ast(AST_ENUMOR_LIST, 2,    // non-terminal
                                                    new_ast(AST_ENUMOR, 1, // non-terminal
                                                            new_identifier_ast(AST_ENUM_CONST, new_string("PLUS"))),
                                                    new_ast(AST_ENUMOR, 1, // non-terminal
                                                            new_identifier_ast(AST_ENUM_CONST, new_string("MINUS")))))),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_unnamed_enum_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("FIRST")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("SECOND")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("THIRD")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("enumeration")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,                                   // non-terminal
                new_ast(AST_DECL_SPECS, 1,                     // non-terminal
                        new_ast(AST_TYPE_ENUM, 1,              // non-terminal
                                new_ast(AST_ENUMOR_LIST, 3,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("FIRST"))),
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("SECOND"))),
                                        new_ast(AST_ENUMOR, 2, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("THIRD")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))))),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_nameonly_enum_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Enum")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("enumeration")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,                      // non-terminal
                            new_ast(AST_DECL_SPECS, 1,        // non-terminal
                                    new_ast(AST_TYPE_ENUM, 1, // non-terminal
                                            new_identifier_ast(AST_ENUM_NAME, new_string("Enum")))),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("enumeration")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_enum_name_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_ENUM));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("Enum")));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("FIRST")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("SECOND")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("THIRD")));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 1,                      // non-terminal
                new_ast(AST_DECL_SPECS, 1,        // non-terminal
                        new_ast(AST_TYPE_ENUM, 2, // non-terminal
                                new_identifier_ast(AST_ENUM_NAME, new_string("Enum")),
                                new_ast(AST_ENUMOR_LIST, 3,    // non-terminal
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("FIRST"))),
                                        new_ast(AST_ENUMOR, 2, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("SECOND")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))),
                                        new_ast(AST_ENUMOR, 1, // non-terminal
                                                new_identifier_ast(AST_ENUM_CONST, new_string("THIRD")))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_typedef_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_TYPEDEF));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("test_type")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 2, // non-terminal
                                    new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_CHAR, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("test_type"))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_typedef_name_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("test_type")));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Set* typedef_names_set = new_set(&t_hashable_string);
    set_add(typedef_names_set, new_string("test_type"));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                    new_identifier_ast(AST_TYPEDEF_NAME, new_string("test_type"))),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));

    run_decl_parser_test(input, typedef_names_set, expected);

    delete_ast(expected);
}

void test_parse_enclosed_decl_int(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("i")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                    new_ast(AST_TYPE_INT, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("i")))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_enclosed_decl_pointer(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("p")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(AST_DECL, 2,               // non-terminal
                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                    new_ast(AST_TYPE_CHAR, 0)),
                            new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                    new_ast(AST_INIT_DECLOR, 1,        // non-terminal
                                            new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_enclosed_decl_array(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("apfi")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("y")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1, // non-terminal
            new_ast(
                AST_INIT_DECLOR, 1, // non-terminal
                new_ast(
                    AST_ARRAY_DECLOR, 2,                // non-terminal
                    new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                            new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("apfi")),
                                    new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                            new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                    new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                            new_ast(AST_TYPE_INT, 0)),
                                                    new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))),
                                            new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                    new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                            new_ast(AST_TYPE_INT, 0)),
                                                    new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))))))),
                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_enclosed_decl_function(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("signal")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("sig")));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("func")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_VOID, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,                            // non-terminal
            new_ast(AST_INIT_DECLOR, 1,                         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2,                 // non-terminal
                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("signal")),
                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                    new_ast(AST_TYPE_INT, 0)),
                                                            new_ast(AST_ABS_DECLOR, 0))))),
                            new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("sig"))),
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_VOID, 0)),
                                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("func")),
                                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                    new_ast(AST_TYPE_INT, 0)),
                                                                            new_ast(AST_ABS_DECLOR, 0)))))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_enclosed_decl_abstract_params(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("signal")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_VOID, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,                            // non-terminal
            new_ast(AST_INIT_DECLOR, 1,                         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2,                 // non-terminal
                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("signal")),
                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                    new_ast(AST_TYPE_INT, 0)),
                                                            new_ast(AST_ABS_DECLOR, 0))))),
                            new_ast(AST_PARAM_LIST, 2,                 // non-terminal
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_ast(AST_ABS_DECLOR, 0)),
                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                    new_ast(AST_TYPE_VOID, 0)),
                                            new_ast(AST_PTR_DECLOR, 1,          // non-terminal
                                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                            new_ast(AST_ABS_DECLOR, 0),
                                                            new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                                    new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                                            new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                                    new_ast(AST_TYPE_INT, 0)),
                                                                            new_ast(AST_ABS_DECLOR, 0)))))))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_expr_init(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("scalar")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void test_parse_list_init(void) {
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

    Ast* expected =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
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

void test_parse_list_init_comma(void) {
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

    Ast* expected =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))),
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    run_decl_parser_test(input, NULL, expected);

    delete_ast(expected);
}

void run_decl_parser_test(Vector* input, Set* typedef_names_set, Ast* expected) {
    Parser* parser = new_parser(input);
    if (typedef_names_set != NULL) {
        delete_set(parser->typedef_names_set);
        parser->typedef_names_set = typedef_names_set;
    }

    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_decl(parser));

    testlib_assert_ast_equal(actual, expected);
    CU_ASSERT_PTR_NULL(err);

    if (actual != NULL) {
        delete_ast(actual);
    }
    delete_parser(parser);
}
