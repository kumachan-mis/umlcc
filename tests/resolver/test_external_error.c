#include "./test_external_error.h"
#include "../../src/resolver/resolver.h"
#include "../testlib/testlib.h"

void test_resolve_translation_unit_error_child(void);
void test_resolve_function_definition_error_decl_specifier(void);
void test_resolve_function_definition_error_storage_specifier(void);
void test_resolve_function_definition_error_non_func(void);
void test_resolve_function_definition_error_declarator(void);
void test_resolve_function_definition_error_duplicated(void);
void test_resolve_function_definition_error_body(void);

void run_resolver_error_test(Ast* input, Vector* expected);

CU_Suite* add_test_suite_external_resolver_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_external_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_translation_unit_error_child);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_decl_specifier);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_storage_specifier);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_non_func);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_declarator);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_function_definition_error_body);
    return suite;
}

void test_resolve_translation_unit_error_child(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: storage specifiers are invalid for a function parameter\n"));
    vector_push(expected, new_error("Error: expression is not assignable to function return\n"));

    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void test_resolve_function_definition_error_decl_specifier(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: declaration specifiers are invalid\n"));

    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void test_resolve_function_definition_error_storage_specifier(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: storage specifiers are invalid for a function definition\n"));
    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void test_resolve_function_definition_error_non_func(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: non-function declaration should not have body\n"));
    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void test_resolve_function_definition_error_declarator(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: parameter 'x' is already declared\n"));
    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void test_resolve_function_definition_error_duplicated(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: identifier 'error' is already declared\n"));
    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void test_resolve_function_definition_error_body(void) {
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("Error: identifier 'x' is already declared\n"));
    vector_push(expected, new_error("Error: left operand of assignment should be modifiable lvalue\n"));
    run_resolver_error_test(input, expected);

    delete_vector(expected);
}

void run_resolver_error_test(Ast* input, Vector* expected) {
    Resolver* resolver = new_resolver(input);

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolver_resolve_semantics(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}
