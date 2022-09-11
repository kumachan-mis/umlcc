#include "./test_expression.h"
#include "../../src/resolver/expression.h"
#include "../testlib/testlib.h"

void test_resolve_assignment_expr();
void test_resolve_logical_or_expr();
void test_resolve_logical_and_expr();
void test_resolve_equal_expr();
void test_resolve_not_equal_expr();
void test_resolve_add_expr();
void test_resolve_subtract_expr();
void test_resolve_pointer_add_expr();
void test_resolve_pointer_subtract_expr();
void test_resolve_pointer_difference_expr();
void test_resolve_multiply_expr();
void test_resolve_division_expr();
void test_resolve_modulo_expr();
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

void run_expr_resolver_test(Ast* __restrict__ input, SymbolTable* __restrict__ local_table,
                            SymbolTable* __restrict__ global_table, Srt* __restrict__ expected,
                            Srt* __restrict__ expected_trans_unit);

CU_Suite* add_test_suite_expr_resolver() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_assignment_expr);
    CU_ADD_TEST(suite, test_resolve_logical_or_expr);
    CU_ADD_TEST(suite, test_resolve_logical_and_expr);
    CU_ADD_TEST(suite, test_resolve_equal_expr);
    CU_ADD_TEST(suite, test_resolve_not_equal_expr);
    CU_ADD_TEST(suite, test_resolve_add_expr);
    CU_ADD_TEST(suite, test_resolve_subtract_expr);
    CU_ADD_TEST(suite, test_resolve_pointer_add_expr);
    CU_ADD_TEST(suite, test_resolve_pointer_subtract_expr);
    CU_ADD_TEST(suite, test_resolve_pointer_difference_expr);
    CU_ADD_TEST(suite, test_resolve_multiply_expr);
    CU_ADD_TEST(suite, test_resolve_division_expr);
    CU_ADD_TEST(suite, test_resolve_modulo_expr);
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

void test_resolve_assignment_expr() {
    Ast* input =
        new_ast(AST_ASSIGN_EXPR, 2,        // non-terminal
                new_ast(AST_INDIR_EXPR, 1, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("x"))),
                new_ast(AST_ASSIGN_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("y")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))));

    SymbolTable* local_table = new_symboltable();
    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("x"), pointer_dtype);
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_CHAR), 2, // non-terminal
        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                       1, // non-terminal
                       new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_CHAR),
                                      1, // non-terminal
                                      new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(pointer_dtype),
                                                         new_string("x")))),
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                               1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                  new_string("y"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                 new_signed_iliteral(INTEGER_INT, 0)))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_logical_or_expr() {
    Ast* input = new_ast(
        AST_LOR_EXPR, 2, // non-terminal
        new_ast(
            AST_LOR_EXPR, 2,                   // non-terminal
            new_ast(AST_LOR_EXPR, 2,           // non-terminal
                    new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                            new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                    new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                            new_identifier_ast(AST_IDENT_EXPR, new_string("i")),
                            new_identifier_ast(AST_IDENT_EXPR, new_string("m")))),
            new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                    new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                    new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)))),
        new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                new_identifier_ast(AST_IDENT_EXPR, new_string("j")),
                new_identifier_ast(AST_IDENT_EXPR, new_string("n"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("j"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("m"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_dtyped_srt(
                SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2,                  // non-terminal
                new_dtyped_srt(SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                  new_string("i")),
                               new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                new_signed_iliteral(INTEGER_INT, 0))),
                new_dtyped_srt(
                    SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                       new_string("i")),
                    new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                                      new_string("m"))))),
            new_dtyped_srt(
                SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                 new_signed_iliteral(INTEGER_INT, 0)))),
        new_dtyped_srt(
            SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("j")),
            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                              new_string("n")))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_logical_and_expr() {
    Ast* input =
        new_ast(AST_LAND_EXPR, 2,           // non-terminal
                new_ast(AST_NEQUAL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("check")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                new_ast(AST_CALL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("validate")),
                        new_ast(AST_ARG_LIST, 1, // non-terminal
                                new_identifier_ast(AST_IDENT_EXPR, new_string("object")))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("check"), new_integer_dtype(DTYPE_INT));

    Dtype* arg_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6);
    symboltable_define_memory(local_table, new_string("object"), arg_dtype);

    Vector* params = new_vector(&t_dparam);
    Dtype* param_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    vector_push(params, new_dparam(new_string("target"), param_dtype));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));
    symboltable_define_label(local_table, new_string("validate"), func_dtype);

    Srt* expected = new_dtyped_srt(
        SRT_LAND_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_NEQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("check")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                             new_signed_iliteral(INTEGER_INT, 0))),
        new_dtyped_srt(
            SRT_CALL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_dtyped_srt(
                SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(func_dtype)), 1, // non-terminal
                new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(func_dtype), new_string("validate"))),
            new_srt(SRT_ARG_LIST, 1,                                          // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, dtype_copy(param_dtype), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(arg_dtype),
                                                      new_string("object"))))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_equal_expr() {
    Ast* input = new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("y")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))),
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("y"))));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_not_equal_expr() {
    Ast* input =
        new_ast(AST_NEQUAL_EXPR, 2,        // non-terminal
                new_ast(AST_EQUAL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("zero_flag")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0))),
                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("zero_flag"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_NEQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2,               // non-terminal
        new_dtyped_srt(SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                          new_string("zero_flag")),
                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                        new_signed_iliteral(INTEGER_INT, 0))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 0)));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_add_expr() {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("n")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("n"))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 3)));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_subtract_expr() {
    Ast* input =
        new_ast(AST_SUB_EXPR, 2,         // non-terminal
                new_ast(AST_ADD_EXPR, 2, // non-terminal
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 3)),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4))),
                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 6)));

    Srt* expected =
        new_dtyped_srt(SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2,                // non-terminal
                       new_dtyped_srt(SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_signed_iliteral(INTEGER_INT, 3)),
                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_signed_iliteral(INTEGER_INT, 4))),
                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                        new_signed_iliteral(INTEGER_INT, 6)));

    run_expr_resolver_test(input, NULL, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_add_expr() {
    Ast* input = new_ast(AST_ADD_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    // In current implementation,
    // if a pointer is placed on the right side of add operator, the operands will be swaped
    Srt* expected = new_dtyped_srt(
        SRT_PADD_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                           new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 2)));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_subtract_expr() {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("ptr")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Srt* expected = new_dtyped_srt(
        SRT_PSUB_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                           new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 4)));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_pointer_difference_expr() {
    Ast* input = new_ast(AST_SUB_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("p")),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    // In current implementation, ptrdiff_t is int
    Srt* expected = new_dtyped_srt(
        SRT_PDIFF_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                           new_string("p")),
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                           new_string("q")));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_multiply_expr() {
    Ast* input = new_ast(AST_MUL_EXPR, 2, // non-terminal
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 4)),
                         new_identifier_ast(AST_IDENT_EXPR, new_string("n")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_INT));

    Srt* expected = new_dtyped_srt(
        SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 4)),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("n")));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_division_expr() {
    Ast* input = new_ast(AST_DIV_EXPR, 2, // non-terminal
                         new_identifier_ast(AST_IDENT_EXPR, new_string("amount")),
                         new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 2)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("amount"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_DIV_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                          new_string("amount"))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 2)));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
}

void test_resolve_modulo_expr() {
    Ast* input =
        new_ast(AST_MOD_EXPR, 2,         // non-terminal
                new_ast(AST_MUL_EXPR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_EXPR, new_string("value")),
                        new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 9))),
                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 5)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"), new_integer_dtype(DTYPE_CHAR));

    // The usual arithmetic conversions should be performed
    Srt* expected = new_dtyped_srt(
        SRT_MOD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                              new_string("value"))),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                             new_signed_iliteral(INTEGER_INT, 9))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 5)));

    run_expr_resolver_test(input, local_table, NULL, expected, NULL);

    delete_srt(expected);
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
        SRT_IDENT_EXPR, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6), new_string(".SL0"));

    Srt* expected_trans_unit = new_srt(
        SRT_TRAS_UNIT, 1, // non-terminal
        new_srt(
            SRT_DECL_LIST, 1,         // non-terminal
            new_srt(SRT_INIT_DECL, 2, // non-terminal
                    new_identifier_srt(SRT_DECL, new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6),
                                       new_string(".SL0")),
                    new_srt(SRT_INIT, 1, // non-terminal
                            new_sliteral_srt(SRT_STRING_EXPR,
                                             new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6),
                                             new_sliteral(new_string("Hello"), 6))))));

    run_expr_resolver_test(input, NULL, NULL, expected, expected_trans_unit);

    delete_srt(expected);
    delete_srt(expected_trans_unit);
}

void run_expr_resolver_test(Ast* __restrict__ input, SymbolTable* __restrict__ local_table,
                            SymbolTable* __restrict__ global_table, Srt* __restrict__ expected,
                            Srt* __restrict__ expected_trans_unit) {
    Resolver* resolver = new_resolver(input);
    resolver->trans_unit_srt = new_srt(SRT_TRAS_UNIT, 0);
    if (global_table != NULL) {
        delete_symboltable(resolver->global_table);
        resolver->global_table = global_table;
    }
    if (local_table != NULL) resolver->local_table = local_table;

    Srt* actual = NULL;
    Vector* errs = NULL;
    resolverret_assign(&actual, &errs, resolve_expr(resolver));

    CU_ASSERT_TRUE(testlib_srt_equals(actual, expected));
    CU_ASSERT_PTR_NULL(errs);
    if (expected_trans_unit != NULL) {
        CU_ASSERT_TRUE(testlib_srt_equals(resolver->trans_unit_srt, expected_trans_unit));
    }

    delete_srt(actual);
    delete_resolver(resolver);
}
