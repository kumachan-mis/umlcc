#include "./test_declaration_error.h"
#include "../../src/resolver/declaration.h"
#include "../testlib/testlib.h"

void test_resolve_decl_specifiers_error();
void test_resolve_declarator_list_error_duplicated();
void test_resolve_declarator_error();
void test_resolve_array_size_limitation_error();
void test_resolve_parameter_list_error_duplicated();
void test_resolve_parameter_error_invalid_storage_specifier();
void test_resolve_parameter_error_invalid_declarator();
void test_resolve_initializer_error_unassignable();
void test_resolve_initializer_error_non_object();
void test_resolve_initializer_error_nested_list_scalar();
void test_resolve_initializer_error_empty_scalar();
void test_resolve_initializer_error_too_long_scalar();
void test_resolve_initializer_error_scalar_array();
void test_resolve_initializer_error_empty_array();
void test_resolve_initializer_error_too_long_array();
void test_resolve_initializer_error_child_array();
void test_resolve_initializer_error_too_long_nested_array();

void run_local_decl_resolver_error_test(Ast* __restrict__ input,
                                        SymbolTable* __restrict__ local_table,
                                        Vector* __restrict__ messages);
void run_global_decl_resolver_error_test(Ast* __restrict__ input,
                                         SymbolTable* __restrict__ global_table,
                                         Vector* __restrict__ messages);

CU_Suite* add_test_suite_decl_resolver_error() {
    CU_Suite* suite = CU_add_suite("test_suite_decl_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_decl_specifiers_error);
    CU_ADD_TEST(suite, test_resolve_declarator_list_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_declarator_error);
    CU_ADD_TEST(suite, test_resolve_array_size_limitation_error);
    CU_ADD_TEST(suite, test_resolve_parameter_list_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_parameter_error_invalid_storage_specifier);
    CU_ADD_TEST(suite, test_resolve_parameter_error_invalid_declarator);
    CU_ADD_TEST(suite, test_resolve_initializer_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_initializer_error_non_object);
    CU_ADD_TEST(suite, test_resolve_initializer_error_nested_list_scalar);
    CU_ADD_TEST(suite, test_resolve_initializer_error_empty_scalar);
    CU_ADD_TEST(suite, test_resolve_initializer_error_too_long_scalar);
    CU_ADD_TEST(suite, test_resolve_initializer_error_scalar_array);
    CU_ADD_TEST(suite, test_resolve_initializer_error_empty_array);
    CU_ADD_TEST(suite, test_resolve_initializer_error_too_long_array);
    CU_ADD_TEST(suite, test_resolve_initializer_error_child_array);
    CU_ADD_TEST(suite, test_resolve_initializer_error_too_long_nested_array);
    return suite;
}

void test_resolve_decl_specifiers_error() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 2, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0), new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: declaration specifiers are invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_declarator_list_error_duplicated() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 5,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                        new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: identifier 'y' is already declared\n"));
    vector_push(messages, new_string("Error: identifier 'x' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_declarator_error() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 3,             // non-terminal
            new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_ast(AST_FUNC_DECLOR, 2,
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                    new_ast(AST_PARAM_LIST, 0)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)))),
            new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                            new_ast(AST_FUNC_DECLOR, 2,
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("b")),
                                    new_ast(AST_PARAM_LIST, 0)),
                            new_ast(AST_PARAM_LIST, 0))),
            new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2,          // non-terminal
                            new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("c")),
                                    new_iliteral_ast(AST_INT_EXPR,
                                                     new_signed_iliteral(INTEGER_INT, 6))),
                            new_ast(AST_PARAM_LIST, 0)))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: array of functions is invalid\n"));
    vector_push(messages, new_string("Error: function returning function is invalid\n"));
    vector_push(messages, new_string("Error: function returning array is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_array_size_limitation_error() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                new_ast(AST_MUL_EXPR, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 4)),
                                        new_iliteral_ast(AST_INT_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 5)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: only direct integer is supported as array size\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_parameter_list_error_duplicated() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,            // non-terminal
            new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                            new_ast(AST_PARAM_LIST, 5,                      // non-terminal
                                    new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                    new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                    new_ast(AST_TYPE_CHAR, 0)),
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                    new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                    new_ast(AST_TYPE_CHAR, 0)),
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                    new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                    new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_ast(AST_PTR_DECLOR, 1,
                                                    new_identifier_ast(AST_IDENT_DECLOR,
                                                                       new_string("z")))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: parameter name 'x' is duplicated\n"));
    vector_push(messages, new_string("Error: parameter name 'y' is duplicated\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_parameter_error_invalid_storage_specifier() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                new_ast(AST_DECL_SPECIFIERS, 2, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0),
                                                        new_ast(AST_STG_TYPEDEF, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR,
                                                                   new_string("x"))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: storage specifiers are invalid for a function parameter\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_parameter_error_invalid_declarator() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,            // non-terminal
            new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                            new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                    new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                    new_ast(AST_TYPE_INT, 0)),
                                            new_ast(AST_ARRAY_DECLOR, 2,        // non-terminal
                                                    new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                            new_identifier_ast(AST_IDENT_DECLOR,
                                                                               new_string("x")),
                                                            new_ast(AST_PARAM_LIST, 0)),
                                                    new_iliteral_ast(
                                                        AST_INT_EXPR,
                                                        new_signed_iliteral(INTEGER_INT, 5)))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: array of functions is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_unassignable() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: expression is not assignable to declared object\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_non_object() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,                   // non-terminal
                new_ast(AST_INIT_DECLOR, 2,                // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                new_ast(AST_PARAM_LIST, 0)),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: function cannot be initialized\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_nested_list_scalar() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 9)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_empty_scalar() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_INIT_LIST, 0))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: one or more initializer is required in an initializer list\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_too_long_scalar() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,    // non-terminal
            new_ast(AST_INIT_DECLOR, 2, // non-terminal
                    new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                    new_ast(AST_INIT_LIST, 2, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_scalar_array() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,             // non-terminal
            new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: an array should be initialized with an initializer list\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_empty_array() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,             // non-terminal
            new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                    new_ast(AST_INIT_LIST, 0))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: one or more initializer is required in an initializer list\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_too_long_array() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1, // non-terminal
            new_ast(
                AST_INIT_DECLOR, 2,          // non-terminal
                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                new_ast(AST_INIT_LIST, 3, // non-terminal
                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88)),
                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89)),
                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_child_array() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1, // non-terminal
            new_ast(
                AST_INIT_DECLOR, 2,          // non-terminal
                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                new_ast(AST_INIT_LIST, 3,                 // non-terminal
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_CHAR_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 88)))),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_CHAR_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 89)))),
                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages,
                new_string("Error: expression or that enclosed with braces is required\n"));
    vector_push(messages,
                new_string("Error: expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void test_resolve_initializer_error_too_long_nested_array() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(
            AST_INIT_DECLOR_LIST, 1,                     // non-terminal
            new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                    new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                            new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                    new_iliteral_ast(AST_INT_EXPR,
                                                     new_signed_iliteral(INTEGER_INT, 2))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                    new_ast(AST_INIT_LIST, 4,         // non-terminal
                            new_ast(AST_INIT_LIST, 1, // non-terminal
                                    new_iliteral_ast(AST_INT_EXPR,
                                                     new_signed_iliteral(INTEGER_INT, 6))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));
    Ast* global_input = ast_copy(local_input);

    Vector* messages = new_vector(&t_string);
    vector_push(messages, new_string("Error: initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, messages);
    run_global_decl_resolver_error_test(global_input, NULL, messages);

    delete_vector(messages);
}

void run_local_decl_resolver_error_test(Ast* __restrict__ input,
                                        SymbolTable* __restrict__ local_table,
                                        Vector* __restrict__ messages) {
    Resolver* resolver = new_resolver(input);
    if (local_table != NULL) {
        resolver->local_table = local_table;
    } else {
        resolver->local_table = new_symboltable();
    }
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    input = ast_copy(resolver->ast);

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_decl(resolver));

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

void run_global_decl_resolver_error_test(Ast* __restrict__ input,
                                         SymbolTable* __restrict__ global_table,
                                         Vector* __restrict__ messages) {
    Resolver* resolver = new_resolver(input);
    if (global_table != NULL) {
        delete_symboltable(resolver->global_table);
        resolver->global_table = global_table;
    }
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_decl(resolver));

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
