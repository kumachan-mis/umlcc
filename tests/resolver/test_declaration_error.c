#include "./test_declaration_error.h"
#include "../../src/resolver/declaration.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_resolve_decl_specifiers_error(void);
void test_resolve_declarator_list_error_duplicated(void);
void test_resolve_declarator_error(void);
void test_resolve_array_error_size_non_positive(void);
void test_resolve_array_error_size_limit(void);
void test_resolve_param_list_error_duplicated(void);
void test_resolve_param_error_storage_specifier(void);
void test_resolve_param_error_declarator(void);
void test_resolve_init_error_unassignable(void);
void test_resolve_init_error_function(void);
void test_resolve_init_error_typedef_name(void);
void test_resolve_init_error_nested_list_scalar(void);
void test_resolve_init_error_too_long_scalar(void);
void test_resolve_init_error_scalar_array(void);
void test_resolve_init_error_too_long_array(void);
void test_resolve_init_error_too_long_sliteral(void);
void test_resolve_init_error_array_child(void);
void test_resolve_init_error_too_long_nested_array(void);

void run_local_decl_resolver_error_test(Ast* input, SymbolTable* local_table, Vector* expected);
void run_global_decl_resolver_error_test(Ast* input, SymbolTable* global_table, Vector* expected);

CU_Suite* add_test_suite_decl_resolver_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_decl_resolver_error", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_decl_specifiers_error);
    CU_ADD_TEST(suite, test_resolve_declarator_list_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_declarator_error);
    CU_ADD_TEST(suite, test_resolve_array_error_size_non_positive);
    CU_ADD_TEST(suite, test_resolve_array_error_size_limit);
    CU_ADD_TEST(suite, test_resolve_param_list_error_duplicated);
    CU_ADD_TEST(suite, test_resolve_param_error_storage_specifier);
    CU_ADD_TEST(suite, test_resolve_param_error_declarator);
    CU_ADD_TEST(suite, test_resolve_init_error_unassignable);
    CU_ADD_TEST(suite, test_resolve_init_error_function);
    CU_ADD_TEST(suite, test_resolve_init_error_typedef_name);
    CU_ADD_TEST(suite, test_resolve_init_error_nested_list_scalar);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_scalar);
    CU_ADD_TEST(suite, test_resolve_init_error_scalar_array);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_array);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_sliteral);
    CU_ADD_TEST(suite, test_resolve_init_error_array_child);
    CU_ADD_TEST(suite, test_resolve_init_error_too_long_nested_array);
    return suite;
}

void test_resolve_decl_specifiers_error(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 2, // non-terminal
                                       new_ast(AST_TYPE_CHAR, 0), new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("declaration specifiers are invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_declarator_list_error_duplicated(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
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

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("identifier 'y' is already declared\n"));
    vector_push(expected, new_error("identifier 'x' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_declarator_error(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 3,             // non-terminal
                new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                        new_ast(AST_PARAM_LIST, 0)),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)))),
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, new_identifier_ast(AST_IDENT_DECLOR, new_string("b")),
                                        new_ast(AST_PARAM_LIST, 0)),
                                new_ast(AST_PARAM_LIST, 0))),
                new_ast(AST_INIT_DECLOR, 1,                  // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("c")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                new_ast(AST_PARAM_LIST, 0)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array of functions is invalid\n"));
    vector_push(expected, new_error("function returning function is invalid\n"));
    vector_push(expected, new_error("function returning array is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_array_error_size_non_positive(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array size should be a positive integer\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_array_error_size_limit(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 1,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("a")),
                                new_ast(AST_MUL_EXPR, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("only direct integer is supported as array size\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_list_error_duplicated(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 5,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_CHAR, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("y"))),
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_PTR_DECLOR, 1,
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("z")))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("parameter 'x' is already declared\n"));
    vector_push(expected, new_error("parameter 'y' is already declared\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_error_storage_specifier(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                        new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                        new_ast(AST_DECL_SPECS, 2, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0), new_ast(AST_STG_TYPEDEF, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("storage specifiers are invalid for a function parameter\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_param_error_declarator(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f")),
                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_ast(AST_ARRAY_DECLOR, 2,        // non-terminal
                                                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x")),
                                                                new_ast(AST_PARAM_LIST, 0)),
                                                        new_iliteral_ast(AST_INT_EXPR,
                                                                         new_signed_iliteral(INTEGER_INT, 5)))))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("array of functions is invalid\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_unassignable(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,           // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2,        // non-terminal
                                               new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                       new_identifier_ast(AST_IDENT_DECLOR, new_string("p"))),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression is not assignable to declared object\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_function(void) {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 1, // non-terminal
                                       new_ast(AST_TYPE_CHAR, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,                   // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2,                // non-terminal
                                               new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                                       new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                                       new_ast(AST_PARAM_LIST, 0)),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("function cannot be initialized\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_typedef_name() {
    Ast* local_input = new_ast(AST_DECL, 2,               // non-terminal
                               new_ast(AST_DECL_SPECS, 2, // non-terminal
                                       new_ast(AST_STG_TYPEDEF, 0), new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("tint")),
                                               new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("typedef-name cannot be initialized\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_nested_list_scalar(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                new_ast(AST_INIT_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_scalar(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_INIT_LIST, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_scalar_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("an array should be initialized with an initializer list\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88)),
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89)),
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_sliteral(void) {
    char sliteral_const[5] = "test";
    int sliteral_size = 5, array_size = 3;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, array_size))),
                                new_sliteral_ast(AST_STRING_EXPR, sliteral))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer string literal is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_array_child(void) {
    Ast* local_input = new_ast(
        AST_DECL, 2,               // non-terminal
        new_ast(AST_DECL_SPECS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                        new_ast(AST_INIT_LIST, 3,                 // non-terminal
                                new_ast(AST_INIT_LIST, 1,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 88)))),
                                new_ast(AST_INIT_LIST, 1,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89)))),
                                new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 90))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));
    vector_push(expected, new_error("expression or that enclosed with braces is required\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void test_resolve_init_error_too_long_nested_array(void) {
    Ast* local_input =
        new_ast(AST_DECL, 2,               // non-terminal
                new_ast(AST_DECL_SPECS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 4,         // non-terminal
                                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));
    Ast* global_input = ast_copy(local_input);

    Vector* expected = new_vector(&t_error);
    vector_push(expected, new_error("initializer list is too long\n"));

    run_local_decl_resolver_error_test(local_input, NULL, expected);
    run_global_decl_resolver_error_test(global_input, NULL, expected);

    delete_vector(expected);
}

void run_local_decl_resolver_error_test(Ast* input, SymbolTable* local_table, Vector* expected) {
    Resolver* resolver = new_resolver(input);
    if (local_table != NULL) {
        resolver->local_table = local_table;
    } else {
        resolver->local_table = new_symboltable();
    }
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_decl(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}

void run_global_decl_resolver_error_test(Ast* input, SymbolTable* global_table, Vector* expected) {
    Resolver* resolver = new_resolver(input);
    if (global_table != NULL) {
        delete_symboltable(resolver->global_table);
        resolver->global_table = global_table;
    }
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);

    Srt* ret = NULL;
    Vector* actual = NULL;
    resolverret_assign(&ret, &actual, resolve_decl(resolver));

    CU_ASSERT_PTR_NULL(ret);
    testlib_assert_errors_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_resolver(resolver);
}
