#include "./test_expression.h"
#include "../../src/immcgen/expression.h"
#include "../testlib/testlib.h"

void test_immcgen_address_expr_ident();
void test_immcgen_address_expr_indir();
void test_immcgen_indirection_expr();
void test_immcgen_logical_not_expr();
void test_immcgen_call_expr();
void test_immcgen_ident_expr_local();
void test_immcgen_ident_expr_global();
void test_immcgen_iliteral_expr_int();
void test_immcgen_iliteral_expr_char();

void run_expr_immcgen_test(Srt* __restrict__ input, SymbolTable* __restrict__ local_table,
                           SymbolTable* __restrict__ global_table, Vector* __restrict__ expected);

CU_Suite* add_test_suite_expr_immcgen() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_immcgen", NULL, NULL);
    CU_ADD_TEST(suite, test_immcgen_address_expr_ident);
    CU_ADD_TEST(suite, test_immcgen_address_expr_indir);
    CU_ADD_TEST(suite, test_immcgen_indirection_expr);
    CU_ADD_TEST(suite, test_immcgen_logical_not_expr);
    CU_ADD_TEST(suite, test_immcgen_call_expr);
    CU_ADD_TEST(suite, test_immcgen_ident_expr_local);
    CU_ADD_TEST(suite, test_immcgen_ident_expr_global);
    CU_ADD_TEST(suite, test_immcgen_iliteral_expr_int);
    CU_ADD_TEST(suite, test_immcgen_iliteral_expr_char);
    return suite;
}

void test_immcgen_address_expr_ident() {
    Srt* input = new_dtyped_srt(
        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_inst_immc(IMMC_INST_ADDR, new_reg_immcope(IMMC_SUFFIX_QUAD, 0),
                                        new_mem_immcope(4), NULL));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_address_expr_indir() {
    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 5);
    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    Srt* input = new_dtyped_srt(
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

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("a"), array_dtype);

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_inst_immc(IMMC_INST_ADDR, new_reg_immcope(IMMC_SUFFIX_QUAD, 0),
                                        new_mem_immcope(20), NULL));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_MUL, new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                                        new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                                        new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4)));
    vector_push(expected, new_inst_immc(IMMC_INST_ADD, new_reg_immcope(IMMC_SUFFIX_QUAD, 2),
                                        new_reg_immcope(IMMC_SUFFIX_QUAD, 0),
                                        new_reg_immcope(IMMC_SUFFIX_LONG, 1)));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_indirection_expr() {
    Srt* input = new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                                new_identifier_srt(SRT_IDENT_EXPR,
                                                   new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                   new_string("ptr")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_QUAD, 0),
                                        new_mem_immcope(8), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                                        new_ptr_immcope(0), NULL));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_logical_not_expr() {
    Srt* input = new_dtyped_srt(
        SRT_LNOT_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
        new_dtyped_srt(
            SRT_LNOT_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("flag"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("flag"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_BYTE, 0),
                                        new_mem_immcope(1), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_SETEQ, new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                                        new_reg_immcope(IMMC_SUFFIX_BYTE, 0),
                                        new_signed_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 0)));
    vector_push(expected, new_inst_immc(IMMC_INST_SETEQ, new_reg_immcope(IMMC_SUFFIX_LONG, 2),
                                        new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                                        new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0)));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_call_expr() {
    Vector* params = new_vector(&t_dparam);
    vector_push(params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(params, new_dparam(new_string("y"), new_integer_dtype(DTYPE_INT)));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));

    Srt* input = new_dtyped_srt(
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

    SymbolTable* local_table = new_symboltable();
    symboltable_define_label(local_table, new_string("function"), func_dtype);
    symboltable_define_memory(local_table, new_string("a"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_PREP, NULL,
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), NULL));
    vector_push(expected,
                new_inst_immc(IMMC_INST_STARG, new_arg_immcope(IMMC_SUFFIX_LONG, 1),
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_BYTE, 0),
                                        new_mem_immcope(1), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 1),
                                        new_reg_immcope(IMMC_SUFFIX_BYTE, 0), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_STARG, new_arg_immcope(IMMC_SUFFIX_LONG, 0),
                                        new_reg_immcope(IMMC_SUFFIX_LONG, 1), NULL));
    vector_push(expected, new_inst_immc(IMMC_INST_CALL, new_reg_immcope(IMMC_SUFFIX_LONG, 2),
                                        new_label_immcope(new_string("function")),
                                        new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2)));
    vector_push(expected,
                new_inst_immc(IMMC_INST_CLEAN, NULL,
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), NULL));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_ident_expr_local() {
    Srt* input =
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("local"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("local"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_BYTE, 0),
                                        new_mem_immcope(1), NULL));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_ident_expr_global() {
    Srt* input =
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("global"));

    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("global"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 0),
                                        new_label_immcope(new_string("global")), NULL));

    run_expr_immcgen_test(input, NULL, global_table, expected);

    delete_vector(expected);
}

void test_immcgen_iliteral_expr_int() {
    Srt* input = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                  new_signed_iliteral(INTEGER_INT, 3));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 0),
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), NULL));

    run_expr_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_iliteral_expr_char() {
    Srt* input = new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                  new_signed_iliteral(INTEGER_INT, 89));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 0),
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 89), NULL));

    run_expr_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void run_expr_immcgen_test(Srt* __restrict__ input, SymbolTable* __restrict__ local_table,
                           SymbolTable* __restrict__ global_table, Vector* __restrict__ expected) {
    Immcgen* immcgen = new_immcgen(input);
    if (global_table != NULL) {
        delete_symboltable(immcgen->global_table);
        immcgen->global_table = global_table;
    }
    if (local_table != NULL) immcgen->local_table = local_table;

    Vector* actual = immcgen_generate_immcode(immcgen);

    CU_ASSERT_TRUE(testlib_immcs_equals(actual, expected));

    delete_vector(actual);
    delete_immcgen(immcgen);
}
