#include "./test_expression.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

void test_resolve_address_expr();
void test_resolve_indirection_expr();
void test_resolve_logical_not_expr();
void test_resolve_call_expr();
void test_resolve_subscription_expr();
void test_resolve_ident_expr_local();
void test_resolve_ident_expr_global();
void test_resolve_iliteral_expr_int();
void test_resolve_iliteral_expr_char();
void test_resolve_sliteral_expr();

void run_expr_resolver_test(Ast* __restrict__ input, SymbolTable* __restrict__ global_table,
                            SymbolTable* __restrict__ local_table, Srt* __restrict__ expected,
                            Srt* __restrict__ expected_trans_unit);

CU_Suite* add_test_suite_expr_resolver() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_address_expr);
    CU_ADD_TEST(suite, test_resolve_indirection_expr);
    CU_ADD_TEST(suite, test_resolve_logical_not_expr);
    CU_ADD_TEST(suite, test_resolve_call_expr);
    CU_ADD_TEST(suite, test_resolve_subscription_expr);
    CU_ADD_TEST(suite, test_resolve_ident_expr_local);
    CU_ADD_TEST(suite, test_resolve_ident_expr_global);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_int);
    CU_ADD_TEST(suite, test_resolve_iliteral_expr_char);
    CU_ADD_TEST(suite, test_resolve_sliteral_expr);
    return suite;
}

void test_resolve_address_expr() {
    Ast* input =
        new_ast(AST_ADDR_EXPR, 1,          // non-terminal
                new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))));

    SymbolTable* local_table = new_symboltable();
    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("a"), array_dtype);

    Srt* expected = new_dtyped_srt(
        SRT_ADDR_EXPR, dtype_copy(pointer_dtype), 1, // non-terminal
        new_dtyped_srt(
            SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_dtyped_srt(
                SRT_PADD_EXPR, dtype_copy(pointer_dtype), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, dtype_copy(pointer_dtype), 1, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(array_dtype), new_string("a"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                 new_signed_iliteral(INTEGER_INT, 1)))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_indirection_expr() {
    Ast* input = new_ast(AST_INDIR_EXPR, 1, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Srt* expected = new_dtyped_srt(
        SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                           new_string("ptr")));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_logical_not_expr() {
    Ast* input = new_ast(AST_LNOT_EXPR, 1,         // non-terminal
                         new_ast(AST_LNOT_EXPR, 1, // non-terminal
                                 new_identifier_ast(AST_IDENT_EXPR, new_string("flag"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("flag"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_LNOT_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
        new_dtyped_srt(
            SRT_LNOT_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("flag"))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_call_expr() {
    Ast* input =
        new_ast(AST_CALL_EXPR, 2, // non-terminal
                new_identifier_ast(AST_IDENT_EXPR, new_string("function")),
                new_ast(AST_ARG_LIST, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("a")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3))));

    SymbolTable* local_table = new_symboltable();
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_dparam(new_string("y"), new_integer_dtype(DTYPE_INT)));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("a"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected = new_dtyped_srt(
        SRT_CALL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(func_dtype)), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(func_dtype), new_string("function"))),
        new_srt(SRT_ARG_LIST, 2,                                               // non-terminal
                new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                                  new_string("a"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                 new_signed_iliteral(INTEGER_INT, 3))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_subscription_expr() {
    Ast* input = new_ast(AST_SUBSC_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("array")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("array"), array_dtype);

    Srt* expected = new_dtyped_srt(
        SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,                           // non-terminal
        new_dtyped_srt(SRT_PADD_EXPR, dtype_copy(pointer_dtype), 2,                // non-terminal
                       new_dtyped_srt(SRT_ADDR_EXPR, dtype_copy(pointer_dtype), 1, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(array_dtype),
                                                         new_string("array"))),
                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                        new_signed_iliteral(INTEGER_INT, 0))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_ident_expr_local() {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("local"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("local"), new_integer_dtype(DTYPE_CHAR));

    Srt* expected =
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("local"));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_ident_expr_global() {
    Ast* input = new_identifier_ast(AST_IDENT_EXPR, new_string("global"));

    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("global"), new_integer_dtype(DTYPE_INT));

    Srt* expected =
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("global"));

    run_expr_resolver_test(input, NULL, global_table, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_int() {
    Ast* input = new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3));

    Srt* expected = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                     new_signed_iliteral(INTEGER_INT, 3));

    run_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_iliteral_expr_char() {
    Ast* input = new_iliteral_ast(AST_CHAR_EXPR, new_signed_iliteral(INTEGER_INT, 89));

    Srt* expected = new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                     new_signed_iliteral(INTEGER_INT, 89));

    run_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_sliteral_expr() {
    Ast* input = new_sliteral_ast(AST_STRING_EXPR, new_sliteral(new_string("Hello"), 6));

    Srt* expected = new_identifier_srt(
        SRT_IDENT_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6), new_string("LC0"));

    Srt* expected_trans_unit = new_srt(
        SRT_TRAS_UNIT, 1, // non-terminal
        new_srt(
            SRT_DECL_LIST, 1,         // non-terminal
            new_srt(SRT_INIT_DECL, 2, // non-terminal
                    new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6),
                                       new_string("LC0")),
                    new_srt(SRT_INIT, 1, // non-terminal
                            new_sliteral_srt(SRT_STRING_EXPR,
                                             new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6),
                                             new_sliteral(new_string("Hello"), 6))))));

    run_expr_resolver_test(input, NULL, NULL, expected, expected_trans_unit);

    delete_srt(expected);
}

void run_expr_resolver_test(Ast* __restrict__ input, SymbolTable* __restrict__ global_table,
                            SymbolTable* __restrict__ local_table, Srt* __restrict__ expected,
                            Srt* __restrict__ expected_trans_unit) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (global_table != NULL) {
        delete_symboltable(resolver->global_table);
        resolver->global_table = global_table;
    }
    if (local_table != NULL) resolver->local_table = local_table;

    Srt* actual = resolve_expr(resolver);

    CU_ASSERT_TRUE(testlib_srt_equals(actual, expected));
    if (expected_trans_unit != NULL) {
        CU_ASSERT_TRUE(testlib_srt_equals(resolver->trans_unit_srt, expected_trans_unit));
    }

    delete_srt(actual);
    delete_resolver(resolver);
}
