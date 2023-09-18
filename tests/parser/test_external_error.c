#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/external.h"
#include "../testlib/testlib.h"
#include "./test_external.h"

void test_parse_translation_unit_error_empty(void);
void test_parse_external_decl_error(void);
void test_parse_function_definition_error_decl_specifier(void);
void test_parse_function_definition_error_empty_param_specifier(void);
void test_parse_function_definition_error_param_list(void);
void test_parse_function_definition_error_body(void);

void run_parser_error_test(Vector* input, Error* expected);
void run_function_definition_parser_error_test(Vector* input, Error* expected);

CU_Suite* add_test_suite_external_parser_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_external_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_translation_unit_error_empty);
    CU_ADD_TEST(suite, test_parse_external_decl_error);
    CU_ADD_TEST(suite, test_parse_function_definition_error_decl_specifier);
    CU_ADD_TEST(suite, test_parse_function_definition_error_empty_param_specifier);
    CU_ADD_TEST(suite, test_parse_function_definition_error_param_list);
    CU_ADD_TEST(suite, test_parse_function_definition_error_body);
    return suite;
}

void test_parse_translation_unit_error_empty(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("one of declaration specifiers expected, but got EOF");

    run_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_external_decl_error(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Error* expected = new_error("unexpected token int");

    run_parser_error_test(input, expected);

    delete_error(expected);
}

void test_parse_function_definition_error_decl_specifier(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    Error* expected = new_error("one of declaration specifiers expected, but got identifier");

    run_parser_error_test(input, expected);
    run_function_definition_parser_error_test(func_def_inpput, expected);

    delete_error(expected);
}

void test_parse_function_definition_error_empty_param_specifier(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    Error* expected = new_error("one of declaration specifiers expected, but got identifier");

    run_parser_error_test(input, expected);
    run_function_definition_parser_error_test(func_def_inpput, expected);

    delete_error(expected);
}

void test_parse_function_definition_error_param_list(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    Error* expected = new_error("token ) expected, but got EOF");

    run_parser_error_test(input, expected);
    run_function_definition_parser_error_test(func_def_inpput, expected);

    delete_error(expected);
}

void test_parse_function_definition_error_body(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPAREN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_VOID));
    vector_push(input, new_ctoken(CTOKEN_RPAREN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    Error* expected = new_error("unexpected token EOF");

    run_parser_error_test(input, expected);
    run_function_definition_parser_error_test(func_def_inpput, expected);

    delete_error(expected);
}

void run_parser_error_test(Vector* input, Error* expected) {
    Parser* parser = new_parser(input);
    Ast* ret = NULL;
    Error* actual = NULL;
    parserret_assign(&ret, &actual, parser_create_ast(parser));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) {
        delete_error(actual);
    }
    delete_parser(parser);
}

void run_function_definition_parser_error_test(Vector* input, Error* expected) {
    Parser* parser = new_parser(input);
    Ast* ret = NULL;
    Error* actual = NULL;
    parserret_assign(&ret, &actual, parse_function_definition(parser));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) {
        delete_error(actual);
    }
    delete_parser(parser);
}
