#include "./test_statement.h"
#include "../../src/immcgen/statement.h"
#include "../testlib/testlib.h"

void test_immcgen_compound_stmt_integer_vardef();
void test_immcgen_compound_stmt_pointer_typedef();
void test_immcgen_compound_stmt_empty();
void test_immcgen_return_stmt();
void test_immcgen_expression_stmt();

void run_stmt_immcgen_test(Srt* __restrict__ input, SymbolTable* __restrict__ local_table, int return_label_id,
                           Vector* __restrict__ expected);

CU_Suite* add_test_suite_stmt_immcgen() {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_immcgen", NULL, NULL);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_integer_vardef);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_pointer_typedef);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_empty);
    CU_ADD_TEST(suite, test_immcgen_return_stmt);
    CU_ADD_TEST(suite, test_immcgen_expression_stmt);
    return suite;
}

void test_immcgen_compound_stmt_integer_vardef() {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 3, // non-terminal
        new_srt(
            SRT_DECL_LIST, 3,         // non-terminal
            new_srt(SRT_INIT_DECL, 2, // non-terminal
                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("x")),
                    new_srt(SRT_INIT, 1,
                            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                             new_signed_iliteral(INTEGER_INT, 3)))),
            new_srt(SRT_INIT_DECL, 2, // non-terminal
                    new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("y")),
                    new_srt(SRT_INIT, 1,
                            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                             new_signed_iliteral(INTEGER_INT, 1)))),
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("z")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,    // non-terminal
                                                  new_identifier_srt(SRT_IDENT_EXPR,
                                                                     new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                                     new_string("z")))),
                    new_dtyped_srt(SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
                new_dtyped_srt(
                    SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 2)),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                      // inst
                              new_mem_immcope(4),                                   // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                      // inst
                              new_mem_immcope(8),                                   // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(16),                  // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                      // inst
                              new_ptr_immcope(3),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 5), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 6),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 4),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 5))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                      // inst
                              new_mem_immcope(4),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 6), // fst_src
                              NULL));                               // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_pointer_typedef() {
    Dtype* pint_def_dtype = new_decoration_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    pint_def_dtype->decoration->typedef_flag = 1;

    Srt* input = new_srt(
        SRT_CMPD_STMT, 3,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, pint_def_dtype, new_string("pint")))),
        new_srt(
            SRT_DECL_LIST, 2,         // non-terminal
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("p"))),
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("q")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                    new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,    // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      new_string("p")))),
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_INT), 1,    // non-terminal
                                                  new_identifier_srt(SRT_IDENT_EXPR,
                                                                     new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                                     new_string("q")))),
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                     new_signed_iliteral(INTEGER_INT, 7))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 7), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                  // fst_src
                              NULL));                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                      // inst
                              new_ptr_immcope(1),                   // dst
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
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                               // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_empty() {
    Srt* input = new_srt(SRT_CMPD_STMT, 0);

    Vector* expected = new_vector(&t_immc);

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_return_stmt() {
    Srt* input =
        new_srt(SRT_RET_STMT, 1,
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STRET,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                       // inst
                              new_label_immcope(new_string("L0")), // dst
                              NULL,                                // fst_src
                              NULL));                              // snd_src

    run_stmt_immcgen_test(input, NULL, 0, expected);

    delete_vector(expected);
}

void test_immcgen_expression_stmt() {
    Srt* input = new_srt(
        SRT_EXPR_STMT, 1, // non-terminal
        new_dtyped_srt(
            SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
            new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                           new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 12))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                        // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                  // dst
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 12), // fst_src
                              NULL));                                                // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                      // inst
                              new_mem_immcope(4),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                               // snd_src

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void run_stmt_immcgen_test(Srt* __restrict__ input, SymbolTable* __restrict__ local_table, int return_label_id,
                           Vector* __restrict__ expected) {
    Immcgen* immcgen = new_immcgen(input);
    if (local_table != NULL) immcgen->local_table = local_table;
    immcgen->return_label_id = return_label_id;

    Vector* actual = immcgen_generate_immcode(immcgen);

    CU_ASSERT_TRUE(testlib_immcs_equals(actual, expected));

    delete_vector(actual);
    delete_immcgen(immcgen);
}
