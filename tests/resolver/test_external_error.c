#include "./test_external_error.h"
#include "../../src/resolver/resolver.h"
#include "../testlib/testlib.h"

void test_resolve_translation_unit_error_child();
void test_resolve_function_definition_error_declaration_specifier();
void test_resolve_function_definition_error_storage_specifier();
void test_resolve_function_definition_error_non_func();
void test_resolve_function_definition_error_declarator();
void test_resolve_function_definition_error_duplicated();
void test_resolve_function_definition_error_body();

void run_resolver_error_test(Ast* __restrict__ input, Vector* __restrict__ messages);

CU_Suite* add_test_suite_external_resolver_error() {
    CU_Suite* suite = CU_add_suite("test_suite_external_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_translation_unit_error_child);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_declaration_specifier);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_storage_specifier);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_non_func);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_declarator);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_body);
    return suite;
}

void test_resolve_translation_unit_error_child() {
    Ast* input = new_ast(
        AST_TRAS_UNIT, 3,
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("incriment")),
                                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                        new_ast(AST_DECL_SPECIFIERS, 2, // non-terminal
                                                                new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))))))),
        new_ast(AST_FUNC_DEF, 3,                // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("decriment")),
                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))))),
                new_ast(AST_CMPD_STMT, 1,                // non-terminal
                        new_ast(AST_RET_STMT, 1,         // non-terminal
                                new_ast(AST_SUB_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)))))),
        new_ast(AST_FUNC_DEF, 3,                // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("cast")),
                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))),
                new_ast(AST_CMPD_STMT, 1,        // non-terminal
                        new_ast(AST_RET_STMT, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("x"))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: storage specifiers are invalid for a function parameter\n"));
    vector_push(messages, new_string("Error: expression is not assignable to function return\n"));

    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void test_resolve_function_definition_error_declaration_specifier() {
    Ast* input =
        new_ast(AST_TRAS_UNIT, 1,
                new_ast(AST_FUNC_DEF, 3,                // non-terminal
                        new_ast(AST_DECL_SPECIFIERS, 2, // non-terminal
                                new_ast(AST_TYPE_INT, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("error")), new_ast(AST_PARAM_LIST, 0)),
                        new_ast(AST_CMPD_STMT, 1,        // non-terminal
                                new_ast(AST_RET_STMT, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: declaration specifiers are invalid\n"));

    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void test_resolve_function_definition_error_storage_specifier() {
    Ast* input =
        new_ast(AST_TRAS_UNIT, 1,
                new_ast(AST_FUNC_DEF, 3,                // non-terminal
                        new_ast(AST_DECL_SPECIFIERS, 2, // non-terminal
                                new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("error")), new_ast(AST_PARAM_LIST, 0)),
                        new_ast(AST_CMPD_STMT, 1,        // non-terminal
                                new_ast(AST_RET_STMT, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: storage specifiers are invalid for a function definition\n"));
    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void test_resolve_function_definition_error_non_func() {
    Ast* input =
        new_ast(AST_TRAS_UNIT, 1,
                new_ast(AST_FUNC_DEF, 3,                // non-terminal
                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("error")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                        new_ast(AST_CMPD_STMT, 1,        // non-terminal
                                new_ast(AST_RET_STMT, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: declaration of array should not have body like a function\n"));
    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void test_resolve_function_definition_error_declarator() {
    Ast* input = new_ast(AST_TRAS_UNIT, 1,
                         new_ast(AST_FUNC_DEF, 3,                // non-terminal
                                 new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                         new_ast(AST_TYPE_INT, 0)),
                                 new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                         new_identifier_ast(AST_IDENT_DECLOR, new_string("error")),
                                         new_ast(AST_PARAM_LIST, 2,
                                                 new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                         new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                                 new_ast(AST_TYPE_INT, 0)),
                                                         new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                                 new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                         new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                                 new_ast(AST_TYPE_CHAR, 0)),
                                                         new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))),
                                 new_ast(AST_CMPD_STMT, 1,                // non-terminal
                                         new_ast(AST_RET_STMT, 1,         // non-terminal
                                                 new_ast(AST_ADD_EXPR, 2, // non-terminal
                                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: parameter 'x' is already declared\n"));
    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void test_resolve_function_definition_error_duplicated() {
    Ast* input =
        new_ast(AST_TRAS_UNIT, 3,
                new_ast(AST_FUNC_DEF, 3,                // non-terminal
                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("error")), new_ast(AST_PARAM_LIST, 0)),
                        new_ast(AST_CMPD_STMT, 1,        // non-terminal
                                new_ast(AST_RET_STMT, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                new_ast(AST_FUNC_DEF, 3,                // non-terminal
                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("ok")), new_ast(AST_PARAM_LIST, 0)),
                        new_ast(AST_CMPD_STMT, 1,        // non-terminal
                                new_ast(AST_RET_STMT, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))),
                new_ast(AST_FUNC_DEF, 3,                // non-terminal
                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("error")), new_ast(AST_PARAM_LIST, 0)),
                        new_ast(AST_CMPD_STMT, 1,        // non-terminal
                                new_ast(AST_RET_STMT, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'error' is already declared\n"));
    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void test_resolve_function_definition_error_body() {
    Ast* input = new_ast(
        AST_TRAS_UNIT, 1,
        new_ast(AST_FUNC_DEF, 3,                // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("error")), new_ast(AST_PARAM_LIST, 0)),
                new_ast(AST_CMPD_STMT, 3,                       // non-terminal
                        new_ast(AST_DECL, 2,                    // non-terminal
                                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                        new_ast(AST_TYPE_INT, 0)),
                                new_ast(AST_INIT_DECLOR_LIST, 2,    // non-terminal
                                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))),
                        new_ast(AST_EXPR_STMT, 1,                 // non-terminal
                                new_ast(AST_ASSIGN_EXPR, 2,       // non-terminal
                                        new_ast(AST_ADDR_EXPR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                                        new_ast(AST_ADD_EXPR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))),
                        new_ast(AST_RET_STMT, 1, // non-terminal
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'x' is already declared\n"));
    vector_push(messages, new_string("Error: left operand of assignment should be modifiable lvalue\n"));
    run_resolver_error_test(input, messages);

    delete_vector(messages);
}

void run_resolver_error_test(Ast* __restrict__ input, Vector* __restrict__ messages) {
    Resolver* resolver = new_resolver(input);

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolver_resolve_semantics(resolver));

    CU_ASSERT_PTR_NULL(actual);

    CU_ASSERT_EQUAL(vector_size(errs), vector_size(messages));
    int num_messages = vector_size(messages);
    for (int i = 0; i < num_messages; i++) {
        Error* err = vector_at(errs, i);
        char* message = vector_at(messages, i);
        CU_ASSERT_STRING_EQUAL(err->message, message);
    }

    delete_vector(errs);
    delete_resolver(resolver);
}
