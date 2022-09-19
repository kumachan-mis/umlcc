#include "./test_declaration_error.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/declaration.h"
#include "../testlib/testlib.h"

void test_parse_decl_specifier_error_empty(void);
void test_parse_arithmetic_declarator_error(void);
void test_parse_pointer_declarator_error(void);
void test_parse_array_declarator_error_size(void);
void test_parse_array_declarator_error_bracket(void);
void test_parse_function_declarator_error_return_type(void);
void test_parse_function_declarator_error_param_decl(void);
void test_parse_function_declarator_error_param_list(void);
void test_parse_init_expr_error(void);
void test_parse_init_list_error(void);
void test_parse_init_list_error_item(void);

void run_decl_parser_error_test(Vector* input, Error* expected);

CU_Suite* add_test_suite_decl_parser_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_decl_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_decl_specifier_error_empty);
    CU_ADD_TEST(suite, test_parse_arithmetic_declarator_error);
    CU_ADD_TEST(suite, test_parse_pointer_declarator_error);
    CU_ADD_TEST(suite, test_parse_array_declarator_error_size);
    CU_ADD_TEST(suite, test_parse_array_declarator_error_bracket);
    CU_ADD_TEST(suite, test_parse_function_declarator_error_return_type);
    CU_ADD_TEST(suite, test_parse_function_declarator_error_param_decl);
    CU_ADD_TEST(suite, test_parse_function_declarator_error_param_list);
    CU_ADD_TEST(suite, test_parse_init_expr_error);
    CU_ADD_TEST(suite, test_parse_init_list_error);
    CU_ADD_TEST(suite, test_parse_init_list_error_item);
    return suite;
}

void test_parse_decl_specifier_error_empty(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("one of declaration-specifiers expected, but got identifier\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_arithmetic_declarator_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token return\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_pointer_declarator_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token char\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_array_declarator_error_size(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token =\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_array_declarator_error_bracket(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token ] expected, but got }\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_function_declarator_error_return_type(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("one of declaration-specifiers expected, but got identifier\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_function_declarator_error_param_decl(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token EOF\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_function_declarator_error_param_list(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token , expected, but got EOF\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_init_expr_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ;\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_init_list_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_COMMA));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token ,\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_init_list_error_item(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("token , expected, but got integer-constant\n");

    run_decl_parser_error_test(input, expected);

    delete_error(expected);
}

void run_decl_parser_error_test(Vector* input, Error* expected) {
    Parser* parser = new_parser(input);

    Ast* ret = NULL;
    Error* actual = NULL;
    parserret_assign(&ret, &actual, parse_decl(parser));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) delete_error(actual);
    delete_parser(parser);
}
