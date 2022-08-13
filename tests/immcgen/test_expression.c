#include "./test_expression.h"
#include "../../src/immcgen/expression.h"
#include "../testlib/testlib.h"

void test_immcgen_assignment_expr();
void test_immcgen_logical_or_expr();
void test_immcgen_logical_and_expr();
void test_immcgen_equal_expr();
void test_immcgen_not_equal_expr();
void test_immcgen_add_expr();
void test_immcgen_subtract_expr();
void test_immcgen_pointer_add_expr();
void test_immcgen_pointer_subtract_expr();
void test_immcgen_pointer_difference_expr();
void test_immcgen_multiply_expr();
void test_immcgen_division_expr();
void test_immcgen_modulo_expr();
void test_immcgen_address_expr_local();
void test_immcgen_address_expr_global();
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
    CU_ADD_TEST(suite, test_immcgen_assignment_expr);
    CU_ADD_TEST(suite, test_immcgen_logical_or_expr);
    CU_ADD_TEST(suite, test_immcgen_logical_and_expr);
    CU_ADD_TEST(suite, test_immcgen_equal_expr);
    CU_ADD_TEST(suite, test_immcgen_not_equal_expr);
    CU_ADD_TEST(suite, test_immcgen_add_expr);
    CU_ADD_TEST(suite, test_immcgen_subtract_expr);
    CU_ADD_TEST(suite, test_immcgen_pointer_add_expr);
    CU_ADD_TEST(suite, test_immcgen_pointer_subtract_expr);
    CU_ADD_TEST(suite, test_immcgen_pointer_difference_expr);
    CU_ADD_TEST(suite, test_immcgen_multiply_expr);
    CU_ADD_TEST(suite, test_immcgen_division_expr);
    CU_ADD_TEST(suite, test_immcgen_modulo_expr);
    CU_ADD_TEST(suite, test_immcgen_address_expr_local);
    CU_ADD_TEST(suite, test_immcgen_address_expr_global);
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

void test_immcgen_assignment_expr() {
    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    Srt* input = new_dtyped_srt(
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

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), pointer_dtype);
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                      // inst
                              new_mem_immcope(12),                  // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 1), // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                      // inst
                              new_ptr_immcope(2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 1), // fst_src
                              NULL));                               // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_logical_or_expr() {
    Srt* input = new_dtyped_srt(
        SRT_LOR_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                             new_signed_iliteral(INTEGER_INT, 0))),
        new_dtyped_srt(
            SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
            new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                              new_string("n")))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETEQ,                                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JNEQ,                                         // inst
                              new_label_immcope(new_string("L0")),                    // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 3), // dst
                              new_mem_immcope(5),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 3), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETEQ,                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 5),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JNEQ,                                         // inst
                              new_label_immcope(new_string("L0")),                    // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 5),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 6),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_sr
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                       // inst
                              new_label_immcope(new_string("L1")), // dst
                              NULL,                                // fst_src
                              NULL));                              // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 6),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_sr
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("L1")));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_logical_and_expr() {
    Dtype* arg_dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 6);

    Vector* params = new_vector(&t_dparam);
    Dtype* param_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    vector_push(params, new_dparam(new_string("target"), param_dtype));
    Dtype* func_dtype = new_function_dtype(params, new_integer_dtype(DTYPE_INT));

    Srt* input = new_dtyped_srt(
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

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("check"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("object"), arg_dtype);
    symboltable_define_label(local_table, new_string("validate"), func_dtype);

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETNEQ,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JEQ,                                          // inst
                              new_label_immcope(new_string("L0")),                    // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_PREP,                                       // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADDR,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(10),                  // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STARG,                      // inst
                              new_arg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CALL,                                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3),                   // dst
                              new_label_immcope(new_string("validate")),              // fst_src
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CLEAN,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JEQ,                                          // inst
                              new_label_immcope(new_string("L0")),                    // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_sr
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                       // inst
                              new_label_immcope(new_string("L1")), // dst
                              NULL,                                // fst_src
                              NULL));                              // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_sr
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("L1")));

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_equal_expr() {
    Srt* input = new_dtyped_srt(
        SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("x"))),
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("y"))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_CHAR));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 2), // dst
                              new_mem_immcope(2),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 2), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETEQ,                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_not_equal_expr() {
    Srt* input = new_dtyped_srt(
        SRT_NEQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2,               // non-terminal
        new_dtyped_srt(SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                          new_string("zero_flag")),
                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                        new_signed_iliteral(INTEGER_INT, 0))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 0)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("zero_flag"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETEQ,                                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETNEQ,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_add_expr() {
    Srt* input = new_dtyped_srt(
        SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_dtyped_srt(
            SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("n"))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 3)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_subtract_expr() {
    Srt* input =
        new_dtyped_srt(SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2,                // non-terminal
                       new_dtyped_srt(SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_signed_iliteral(INTEGER_INT, 3)),
                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_signed_iliteral(INTEGER_INT, 4))),
                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                        new_signed_iliteral(INTEGER_INT, 6)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 6))); // snd_src

    run_expr_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_pointer_add_expr() {
    Srt* input = new_dtyped_srt(
        SRT_PADD_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                           new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 2)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_pointer_subtract_expr() {
    Srt* input = new_dtyped_srt(
        SRT_PSUB_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                           new_string("ptr")),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 4)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("ptr"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_pointer_difference_expr() {
    Srt* input = new_dtyped_srt(
        SRT_PDIFF_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                           new_string("p")),
        new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)),
                           new_string("q")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("p"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));
    symboltable_define_memory(local_table, new_string("q"),
                              new_pointer_dtype(new_integer_dtype(DTYPE_CHAR)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                  // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_DIV,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_multiply_expr() {
    Srt* input = new_dtyped_srt(
        SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 4)),
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("n")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_division_expr() {
    Srt* input = new_dtyped_srt(
        SRT_DIV_EXPR, new_integer_dtype(DTYPE_INT), 2,                 // non-terminal
        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR),
                                          new_string("amount"))),
        new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                         new_signed_iliteral(INTEGER_INT, 2)));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("amount"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_DIV,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_modulo_expr() {
    Srt* input = new_dtyped_srt(
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

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("value"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 9))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MOD,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 5))); // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_address_expr_local() {
    Srt* input = new_dtyped_srt(
        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADDR,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_address_expr_global() {
    Srt* input = new_dtyped_srt(
        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")));

    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADDR,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_label_immcope(new_string("x")),   // fst_src
                              NULL));                               // snd_src

    run_expr_immcgen_test(input, NULL, global_table, expected);

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
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADDR,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(20),                  // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_ptr_immcope(0),                   // fst_src
                              NULL));                               // snd_src

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
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETEQ,                                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_BYTE, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SETEQ,                                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src

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
                new_inst_immc(IMMC_INST_PREP,                                       // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STARG,                                      // inst
                              new_arg_immcope(IMMC_SUFFIX_LONG, 1),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STARG,                      // inst
                              new_arg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CALL,                                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),                   // dst
                              new_label_immcope(new_string("function")),              // fst_src
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CLEAN,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), // fst_src
                              NULL));                                               // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_ident_expr_local() {
    Srt* input =
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_CHAR), new_string("local"));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("local"), new_integer_dtype(DTYPE_CHAR));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src

    run_expr_immcgen_test(input, local_table, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_ident_expr_global() {
    Srt* input =
        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("global"));

    SymbolTable* global_table = new_symboltable();
    symboltable_define_label(global_table, new_string("global"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),    // dst
                              new_label_immcope(new_string("global")), // fst_src
                              NULL));                                  // snd_src

    run_expr_immcgen_test(input, NULL, global_table, expected);

    delete_vector(expected);
}

void test_immcgen_iliteral_expr_int() {
    Srt* input = new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                  new_signed_iliteral(INTEGER_INT, 3));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                               // snd_src

    run_expr_immcgen_test(input, NULL, NULL, expected);

    delete_vector(expected);
}

void test_immcgen_iliteral_expr_char() {
    Srt* input = new_iliteral_srt(SRT_CHAR_EXPR, new_integer_dtype(DTYPE_INT),
                                  new_signed_iliteral(INTEGER_INT, 89));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                  // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 89), // fst_src
                              NULL));                                                // snd_src

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