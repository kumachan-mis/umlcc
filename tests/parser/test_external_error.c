#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/external.h"
#include "../testlib/testlib.h"
#include "./test_external.h"

void test_parse_external_declaration_error();
void test_parse_function_definition_no_specifier_error();
void test_parse_function_definition_no_param_specifier_error();
void test_parse_function_definition_no_param_identifier_error();
void test_parse_function_definition_param_list_error();
void test_parse_function_definition_body_error();

void run_parser_error_test(Vector* __restrict__ input, const char* message);
void run_function_definition_parser_error_test(Vector* __restrict__ input, const char* message);

CU_Suite* add_test_suite_external_parser_error() {
    CU_Suite* suite = CU_add_suite("test_suite_external_parser_error", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_external_declaration_error);
    CU_ADD_TEST(suite, test_parse_function_definition_no_specifier_error);
    CU_ADD_TEST(suite, test_parse_function_definition_no_param_specifier_error);
    CU_ADD_TEST(suite, test_parse_function_definition_no_param_identifier_error);
    CU_ADD_TEST(suite, test_parse_function_definition_param_list_error);
    CU_ADD_TEST(suite, test_parse_function_definition_body_error);
    return suite;
}

void test_parse_external_declaration_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token int\n";
    run_parser_error_test(input, message);
}

void test_parse_function_definition_no_specifier_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    const char* message = "Error: one of declaration-specifiers expected, but got identifier\n";
    run_parser_error_test(input, message);
    run_function_definition_parser_error_test(func_def_inpput, message);
}

void test_parse_function_definition_no_param_specifier_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    const char* message = "Error: one of declaration-specifiers expected, but got identifier\n";
    run_parser_error_test(input, message);
    run_function_definition_parser_error_test(func_def_inpput, message);
}

void test_parse_function_definition_no_param_identifier_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    const char* message = "Error: unexpected token )\n";
    run_parser_error_test(input, message);
    run_function_definition_parser_error_test(func_def_inpput, message);
}

void test_parse_function_definition_param_list_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Vector* func_def_inpput = vector_copy(input);

    const char* message = "Error: token , expected, but got EOF\n";
    run_parser_error_test(input, message);
    run_function_definition_parser_error_test(func_def_inpput, message);
}

void test_parse_function_definition_body_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
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

    const char* message = "Error: unexpected token EOF\n";
    run_parser_error_test(input, message);
    run_function_definition_parser_error_test(func_def_inpput, message);
}

void run_parser_error_test(Vector* __restrict__ input, const char* message) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parser_create_ast(parser));

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_parser(parser);
}

void run_function_definition_parser_error_test(Vector* __restrict__ input, const char* message) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_function_definition(parser));

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_parser(parser);
}
