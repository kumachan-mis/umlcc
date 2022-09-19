#include "./test_declaration_error.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/declaration.h"
#include "../testlib/testlib.h"

void test_parse_decl_specifier_error_empty();
void test_parse_arithmetic_declarator_error();
void test_parse_pointer_declarator_error();
void test_parse_array_declarator_error_size();
void test_parse_array_declarator_error_bracket();
void test_parse_function_declarator_error_return_type();
void test_parse_function_declarator_error_param_decl();
void test_parse_function_declarator_error_param_list();
void test_parse_init_expr_error();
void test_parse_init_list_error();
void test_parse_init_list_error_item();

void run_decl_parser_error_test(Vector* __restrict__ input, const char* message);

CU_Suite* add_test_suite_decl_parser_error() {
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

void test_parse_decl_specifier_error_empty() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: one of declaration-specifiers expected, but got identifier\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_arithmetic_declarator_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token return\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_pointer_declarator_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_ASTERISK));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token char\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_array_declarator_error_size() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_RBRACKET));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token =\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_array_declarator_error_bracket() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_LBRACKET));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 3)));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token ] expected, but got }\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_function_declarator_error_return_type() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: one of declaration-specifiers expected, but got identifier\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_function_declarator_error_param_decl() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token EOF\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_function_declarator_error_param_list() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("f")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("a")));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: token , expected, but got EOF\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_init_expr_error() {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_CHAR));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_EQUAL));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    const char* message = "Error: unexpected token ;\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_init_list_error() {
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

    const char* message = "Error: unexpected token ,\n";
    run_decl_parser_error_test(input, message);
}

void test_parse_init_list_error_item() {
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

    const char* message = "Error: token , expected, but got integer-constant\n";
    run_decl_parser_error_test(input, message);
}

void run_decl_parser_error_test(Vector* __restrict__ input, const char* message) {
    Parser* parser = new_parser(input);

    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parse_decl(parser));

    CU_ASSERT_PTR_NULL(actual);
    CU_ASSERT_STRING_EQUAL(err->message, message);

    delete_error(err);
    delete_parser(parser);
}
