#include "./test_declaration.h"
#include "../../src/resolver/declaration.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_resolve_char_decl();
void test_resolve_int_decl();
void test_resolve_pointer_decl();
void test_resolve_array_decl();
void test_resolve_function_decl();
void test_resolve_parameter_decl();
void test_resolve_typedef_decl();
void test_resolve_scalar_init();
void test_resolve_scalar_init_enclosed();
void test_resolve_sliteral_init();
void test_resolve_sliteral_init_lacked();
void test_resolve_sliteral_init_enclosed();
void test_resolve_list_init_zero();
void test_resolve_list_init_zero_nested();
void test_resolve_list_init();
void test_resolve_list_init_lacked();
void test_resolve_list_init_nested();
void test_resolve_list_init_nested_lacked();
void test_resolve_list_init_flatten();
void test_resolve_list_init_mix();
void test_resolve_list_init_chararray();

void run_local_decl_resolver_test(Ast* input, SymbolTable* local_table, Srt* expected);
void run_global_decl_resolver_test(Ast* input, SymbolTable* global_table, Srt* expected);

CU_Suite* add_test_suite_decl_resolver() {
    CU_Suite* suite = CU_add_suite("test_suite_decl_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_char_decl);
    CU_ADD_TEST(suite, test_resolve_int_decl);
    CU_ADD_TEST(suite, test_resolve_pointer_decl);
    CU_ADD_TEST(suite, test_resolve_array_decl);
    CU_ADD_TEST(suite, test_resolve_function_decl);
    CU_ADD_TEST(suite, test_resolve_parameter_decl);
    CU_ADD_TEST(suite, test_resolve_typedef_decl);
    CU_ADD_TEST(suite, test_resolve_scalar_init);
    CU_ADD_TEST(suite, test_resolve_scalar_init_enclosed);
    CU_ADD_TEST(suite, test_resolve_sliteral_init);
    CU_ADD_TEST(suite, test_resolve_sliteral_init_lacked);
    CU_ADD_TEST(suite, test_resolve_sliteral_init_enclosed);
    CU_ADD_TEST(suite, test_resolve_list_init_zero);
    CU_ADD_TEST(suite, test_resolve_list_init_zero_nested);
    CU_ADD_TEST(suite, test_resolve_list_init);
    CU_ADD_TEST(suite, test_resolve_list_init_lacked);
    CU_ADD_TEST(suite, test_resolve_list_init_nested);
    CU_ADD_TEST(suite, test_resolve_list_init_nested_lacked);
    CU_ADD_TEST(suite, test_resolve_list_init_flatten);
    CU_ADD_TEST(suite, test_resolve_list_init_mix);
    CU_ADD_TEST(suite, test_resolve_list_init_chararray);
    return suite;
}

void test_resolve_char_decl() {
    Ast* local_input = new_ast(AST_DECL, 2,                    // non-terminal
                               new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                       new_ast(AST_TYPE_CHAR, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("c")))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_CHAR), new_string("c"))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_int_decl() {
    Ast* local_input = new_ast(AST_DECL, 2,                    // non-terminal
                               new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                       new_ast(AST_TYPE_INT, 0)),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("i")))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("i"))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_pointer_decl() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
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
    Ast* global_input = ast_copy(local_input);

    Srt* expected =
        new_srt(SRT_DECL_LIST, 2,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("p"))),
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT))),
                                           new_string("q"))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_array_decl() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 3,             // non-terminal
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
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 3,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 5), new_string("a"))),
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 8),
                                   new_string("b"))),
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 6), 3),
                                   new_string("c"))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_function_decl() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 3,                   // non-terminal
                        new_ast(AST_INIT_DECLOR, 1,                // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                        new_ast(AST_PARAM_LIST, 0))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("g")),
                                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                                new_ast(AST_TYPE_INT, 0)),
                                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("a")))))),
                        new_ast(AST_INIT_DECLOR, 1,         // non-terminal
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
    Ast* global_input = ast_copy(local_input);

    Vector* fparams = new_vector(&t_dparam);
    Dtype* fdtype = new_function_dtype(fparams, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* gparams = new_vector(&t_dparam);
    vector_push(gparams, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    Dtype* gdtype = new_function_dtype(gparams, new_integer_dtype(DTYPE_CHAR));

    Vector* hparams = new_vector(&t_dparam);
    vector_push(hparams, new_dparam(new_string("b"), new_integer_dtype(DTYPE_INT)));
    vector_push(hparams, new_dparam(new_string("c"), new_integer_dtype(DTYPE_INT)));
    Dtype* hdtype = new_function_dtype(hparams, new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_srt(SRT_DECL_LIST, 3,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, fdtype, new_string("f"))),
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, gdtype, new_string("g"))),
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, hdtype, new_string("h"))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_parameter_decl() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("func")),
                                new_ast(AST_PARAM_LIST, 2,                      // non-terminal
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
                                                new_ast(AST_FUNC_DECLOR, 2,        // non-terminal
                                                        new_ast(AST_PTR_DECLOR, 1, // non-terminal
                                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("f"))),
                                                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                                                new_ast(AST_TYPE_INT, 0)),
                                                                        new_identifier_ast(AST_IDENT_DECLOR,
                                                                                           new_string("p")))))))))));
    Ast* global_input = ast_copy(local_input);

    Dtype* adtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    // array parameter is converted to pointer to initial element

    Vector* fparams = new_vector(&t_dparam);
    vector_push(fparams, new_dparam(new_string("p"), new_integer_dtype(DTYPE_INT)));
    Dtype* fdtype = new_pointer_dtype(new_function_dtype(fparams, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR))));
    // function parameter is converted to pointer to function

    Vector* func_params = new_vector(&t_dparam);
    vector_push(func_params, new_dparam(new_string("a"), adtype));
    vector_push(func_params, new_dparam(new_string("f"), fdtype));
    Dtype* func_dtype = new_function_dtype(func_params, new_integer_dtype(DTYPE_INT));

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 1, // non-terminal
                                    new_identifier_srt(SRT_DECL, func_dtype, new_string("func"))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_typedef_decl() {
    Ast* local_input = new_ast(AST_DECL, 2,                    // non-terminal
                               new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                       new_identifier_ast(AST_TYPEDEF_NAME, new_string("test_type"))),
                               new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                                       new_ast(AST_INIT_DECLOR, 1, // non-terminal
                                               new_identifier_ast(AST_IDENT_DECLOR, new_string("x")))));
    Ast* global_input = ast_copy(local_input);

    Dtype* test_dtype = NULL;
    test_dtype = new_decoration_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    test_dtype->decoration->typedef_flag = 1;
    SymbolTable* local_table = new_symboltable();
    symboltable_define_label(local_table, new_string("test_type"), test_dtype);
    SymbolTable* global_table = symboltable_copy(local_table);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 1, // non-terminal
                new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), new_string("x"))));

    run_local_decl_resolver_test(local_input, local_table, expected);
    run_global_decl_resolver_test(global_input, global_table, expected);

    delete_srt(expected);
}

void test_resolve_scalar_init() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));
    Ast* global_input = ast_copy(local_input);

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("scalar")),
                new_srt(SRT_INIT, 1,                                                     // non-terminal
                        new_dtyped_srt(SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                       new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      1, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("x"))),
                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                        new_signed_iliteral(INTEGER_INT, 2))))));

    run_local_decl_resolver_test(local_input, local_table, expected);
    run_global_decl_resolver_test(global_input, global_table, expected);

    delete_srt(expected);
}

void test_resolve_scalar_init_enclosed() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,    // non-terminal
                        new_ast(AST_INIT_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("scalar")),
                                new_ast(AST_INIT_LIST, 1, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(SRT_DECL_LIST, 1,         // non-terminal
                            new_srt(SRT_INIT_DECL, 2, // non-terminal
                                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("scalar")),
                                    new_srt(SRT_INIT, 1, // non-terminal
                                            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                             new_signed_iliteral(INTEGER_INT, 9)))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_sliteral_init() {
    char* sliteral_const = "test";
    int sliteral_size = 5;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, sliteral_size))),
                        new_sliteral_ast(AST_STRING_EXPR, sliteral))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                         sliteral_copy(sliteral)))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_sliteral_init_lacked() {
    char* sliteral_const = "test";
    int sliteral_size = 5, array_size = 10;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);
    StringLiteral* zero_padding_sliteral = sliteral_zero_padding_copy(sliteral, array_size);

    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, array_size))),
                                new_sliteral_ast(AST_STRING_EXPR, sliteral))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), array_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), array_size),
                                         zero_padding_sliteral))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_sliteral_init_enclosed() {
    char* sliteral_const = "test";
    int sliteral_size = 5;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_CHAR, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, sliteral_size))),
                        new_ast(AST_INIT_LIST, 1, // non-terminal
                                new_sliteral_ast(AST_STRING_EXPR, sliteral)))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                   new_string("array")),
                new_srt(SRT_INIT, 1, // non-terminal
                        new_sliteral_srt(SRT_STRING_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), sliteral_size),
                                         sliteral_copy(sliteral)))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_zero() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 2), new_string("array")),
                new_srt(SRT_INIT, 2,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_zero_nested() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 3), new_string("array")),
                new_srt(SRT_INIT, 3,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 1))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 2))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 4))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_lacked() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_INT), 3), new_string("array")),
                new_srt(SRT_INIT, 3,         // non-terminal
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 2))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_nested() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                        new_ast(AST_INIT_LIST, 2,         // non-terminal
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 2))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 4))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 5))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 6)))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_nested_lacked() {
    Ast* local_input = new_ast(
        AST_DECL, 2,                    // non-terminal
        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                new_ast(AST_TYPE_INT, 0)),
        new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                        new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                        new_ast(AST_INIT_LIST, 1,         // non-terminal
                                new_ast(AST_INIT_LIST, 3, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 9))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 5))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_flatten() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,                     // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,                  // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2,         // non-terminal
                                        new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                                new_ast(AST_INIT_LIST, 2, // non-terminal
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 3), 2),
                                   new_string("array")),
                new_srt(SRT_INIT, 2,                 // non-terminal
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 6))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 3,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_mix() {
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
                                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2))),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                    new_ast(AST_INIT_LIST, 3, // non-terminal
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1)),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                            new_ast(AST_INIT_LIST, 1,         // non-terminal
                                    new_ast(AST_INIT_LIST, 1, // non-terminal
                                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5))))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_array_dtype(new_integer_dtype(DTYPE_INT), 2), 3),
                                   new_string("array")),
                new_srt(SRT_INIT, 3,                 // non-terminal
                        new_srt(SRT_INIT, 2,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 1))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 3)))),
                        new_srt(SRT_INIT, 2,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 5))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 2,         // non-terminal
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0))),
                                new_srt(SRT_INIT, 1, // non-terminal
                                        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                         new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void test_resolve_list_init_chararray() {
    Ast* local_input =
        new_ast(AST_DECL, 2,                    // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_CHAR, 0)),
                new_ast(AST_INIT_DECLOR_LIST, 1,             // non-terminal
                        new_ast(AST_INIT_DECLOR, 2,          // non-terminal
                                new_ast(AST_ARRAY_DECLOR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("array")),
                                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))),
                                new_ast(AST_INIT_LIST, 1,
                                        new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 72))))));
    Ast* global_input = ast_copy(local_input);

    Srt* expected = new_srt(
        SRT_DECL_LIST, 1,         // non-terminal
        new_srt(SRT_INIT_DECL, 2, // non-terminal
                new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 3), new_string("array")),
                new_srt(SRT_INIT, 3,                                                            // non-terminal
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 72)))),
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 0)))),
                        new_srt(SRT_INIT, 1,                                                    // non-terminal
                                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                               new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                new_signed_iliteral(INTEGER_INT, 0)))))));

    run_local_decl_resolver_test(local_input, NULL, expected);
    run_global_decl_resolver_test(global_input, NULL, expected);

    delete_srt(expected);
}

void run_local_decl_resolver_test(Ast* input, SymbolTable* local_table, Srt* expected) {
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

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);

    if (actual != NULL) delete_srt(actual);
    delete_resolver(resolver);
}

void run_global_decl_resolver_test(Ast* input, SymbolTable* global_table, Srt* expected) {
    Resolver* resolver = new_resolver(input);
    if (global_table != NULL) {
        delete_symboltable(resolver->global_table);
        resolver->global_table = global_table;
    }
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_decl(resolver));

    testlib_assert_srt_equal(actual, expected);
    CU_ASSERT_PTR_NULL(errs);

    if (actual != NULL) delete_srt(actual);
    delete_resolver(resolver);
}
