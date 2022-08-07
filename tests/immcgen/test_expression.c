#include "./test_expression.h"
#include "../../src/immcgen/expression.h"
#include "../testlib/testlib.h"

void test_immcgen_ident_expr_local();
void test_immcgen_ident_expr_global();
void test_immcgen_iliteral_expr_int();
void test_immcgen_iliteral_expr_char();

void run_expr_immcgen_test(Srt* __restrict__ input, SymbolTable* __restrict__ local_table,
                           SymbolTable* __restrict__ global_table, Vector* __restrict__ expected);

CU_Suite* add_test_suite_expr_immcgen() {
    CU_Suite* suite = CU_add_suite("test_suite_expr_immcgen", NULL, NULL);
    CU_ADD_TEST(suite, test_immcgen_ident_expr_local);
    CU_ADD_TEST(suite, test_immcgen_ident_expr_global);
    CU_ADD_TEST(suite, test_immcgen_iliteral_expr_int);
    CU_ADD_TEST(suite, test_immcgen_iliteral_expr_char);
    return suite;
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
                new_inst_immc(
                    IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 0),
                    new_int_immcope(IMMC_SUFFIX_LONG, new_signed_iliteral(INTEGER_INT, 3)), NULL));

    run_expr_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_iliteral_expr_char() {
    Srt* input = new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                  new_signed_iliteral(INTEGER_INT, 89));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(
                    IMMC_INST_LOAD, new_reg_immcope(IMMC_SUFFIX_LONG, 0),
                    new_int_immcope(IMMC_SUFFIX_LONG, new_signed_iliteral(INTEGER_INT, 89)), NULL));

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
