#include "./test_statement.h"
#include "../../src/immcgen/statement.h"
#include "../testlib/testlib.h"

void test_immcgen_compound_stmt_completed_typedef(void);
void test_immcgen_compound_stmt_int(void);
void test_immcgen_compound_stmt_unsigned_int(void);
void test_immcgen_compound_stmt_long(void);
void test_immcgen_compound_stmt_unsigned_long(void);
void test_immcgen_compound_stmt_long_long(void);
void test_immcgen_compound_stmt_unsigned_long_long(void);
void test_immcgen_compound_stmt_void_pointer(void);
void test_immcgen_compound_stmt_empty(void);
void test_immcgen_return_stmt_non_void(void);
void test_immcgen_return_stmt_void(void);
void test_immcgen_expression_stmt(void);
void test_immcgen_null_stmt(void);
void test_immcgen_if_stmt(void);
void test_immcgen_if_else_stmt(void);
void test_immcgen_if_else_stmt_chain(void);
void test_immcgen_while_stmt(void);
void test_immcgen_for_stmt_init_declaration(void);
void test_immcgen_for_stmt_init_expression(void);
void test_immcgen_for_stmt_all_null(void);

void run_stmt_immcgen_test(Srt* input, SymbolTable* local_table, int return_label_id, Vector* expected);

CU_Suite* add_test_suite_stmt_immcgen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_stmt_immcgen", NULL, NULL);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_completed_typedef);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_int);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_unsigned_int);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_long);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_unsigned_long);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_long_long);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_unsigned_long_long);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_void_pointer);
    CU_ADD_TEST(suite, test_immcgen_compound_stmt_empty);
    CU_ADD_TEST(suite, test_immcgen_return_stmt_non_void);
    CU_ADD_TEST(suite, test_immcgen_return_stmt_void);
    CU_ADD_TEST(suite, test_immcgen_expression_stmt);
    CU_ADD_TEST(suite, test_immcgen_null_stmt);
    CU_ADD_TEST(suite, test_immcgen_if_stmt);
    CU_ADD_TEST(suite, test_immcgen_if_else_stmt);
    CU_ADD_TEST(suite, test_immcgen_if_else_stmt_chain);
    CU_ADD_TEST(suite, test_immcgen_while_stmt);
    CU_ADD_TEST(suite, test_immcgen_for_stmt_init_declaration);
    CU_ADD_TEST(suite, test_immcgen_for_stmt_init_expression);
    CU_ADD_TEST(suite, test_immcgen_for_stmt_all_null);
    return suite;
}

void test_immcgen_compound_stmt_completed_typedef(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 3,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_typedef_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT))),
                                           new_string("pint")))),
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
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 7), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(1),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(2),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_int(void) {
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
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(4),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(8),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(3),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 6),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 6), // fst_src
                              NULL));                                      // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_unsigned_int(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 4u))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_INT, 2u))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)),
                                           new_string("z")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 2, // non-terminal
                    new_dtyped_srt(
                        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)), 1, // non-terminal
                        new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 1,    // non-terminal
                                       new_identifier_srt(SRT_IDENT_EXPR,
                                                          new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_INT)),
                                                          new_string("z")))),
                    new_dtyped_srt(
                        SRT_DIV_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("x")),
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_INT), new_string("y"))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                      // inst
                              new_mem_immcope(4),                                                   // dst
                              new_unsigned_int_immcope(IMMC_SUFFIX_LONG, INTEGER_UNSIGNED_INT, 4u), // fst_src
                              NULL));                                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                      // inst
                              new_mem_immcope(8),                                                   // dst
                              new_unsigned_int_immcope(IMMC_SUFFIX_LONG, INTEGER_UNSIGNED_INT, 2u), // fst_src
                              NULL));                                                               // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                            // fst_src
                              NULL));                                        // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                            // fst_src
                              NULL));                                        // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_DIV,                                   // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_unsigned_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                               // inst
                              new_ptr_immcope(3),                            // dst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                        // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_long(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONG),
                                                 new_signed_iliteral(INTEGER_LONG, 5l))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONG),
                                                 new_signed_iliteral(INTEGER_LONG, 3l))))),
        new_srt(
            SRT_DECL_LIST, 1,         // non-terminal
            new_srt(SRT_INIT_DECL, 1, // non-terminal
                    new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_LONG)), new_string("z")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_LONG), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_LONG)), 1, // non-terminal
                    new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_LONG), 1,    // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_LONG)),
                                                      new_string("z")))),
                new_dtyped_srt(SRT_ADD_EXPR, new_integer_dtype(DTYPE_LONG), 2, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("x")),
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONG), new_string("y"))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                            // inst
                              new_mem_immcope(8),                                         // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_LONG, 5l), // fst_src
                              NULL));                                                     // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                            // inst
                              new_mem_immcope(16),                                        // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_LONG, 3l), // fst_src
                              NULL));                                                     // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(24),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(3),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                                      // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_unsigned_long(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 6ul))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONG, 4ul))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONG)),
                                           new_string("z")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
                    new_dtyped_srt(
                        SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONG)), 1, // non-terminal
                        new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 1,    // non-terminal
                                       new_identifier_srt(SRT_IDENT_EXPR,
                                                          new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONG)),
                                                          new_string("z")))),
                    new_dtyped_srt(
                        SRT_SUB_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), 2, // non-terminal
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("x")),
                        new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONG), new_string("y"))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                        // inst
                              new_mem_immcope(8),                                                     // dst
                              new_unsigned_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_UNSIGNED_LONG, 6ul), // fst_src
                              NULL));                                                                 // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                        // inst
                              new_mem_immcope(16),                                                    // dst
                              new_unsigned_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_UNSIGNED_LONG, 4ul), // fst_src
                              NULL));                                                                 // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                            // fst_src
                              NULL));                                        // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                           // fst_src
                              NULL));                                        // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                                   // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(24),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                               // inst
                              new_ptr_immcope(3),                            // dst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                                        // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_long_long(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONGLONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONGLONG),
                                                 new_signed_iliteral(INTEGER_LONGLONG, 7ll))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_LONGLONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_LONGLONG),
                                                 new_signed_iliteral(INTEGER_LONGLONG, 5ll))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_LONGLONG)),
                                           new_string("z")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_LONGLONG), 2,                                 // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_LONGLONG)), 1, // non-terminal
                               new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_LONGLONG), 1,    // non-terminal
                                              new_identifier_srt(SRT_IDENT_EXPR,
                                                                 new_pointer_dtype(new_integer_dtype(DTYPE_LONGLONG)),
                                                                 new_string("z")))),
                new_dtyped_srt(
                    SRT_ADD_EXPR, new_integer_dtype(DTYPE_LONGLONG), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("x")),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_LONGLONG), new_string("y"))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                 // inst
                              new_mem_immcope(8),                                              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_LONGLONG, 7ll), // fst_src
                              NULL));                                                          // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                 // inst
                              new_mem_immcope(16),                                             // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_LONGLONG, 5ll), // fst_src
                              NULL));                                                          // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(24),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(3),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                                      // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_unsigned_long_long(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("x")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 8ull))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("y")),
                        new_srt(SRT_INIT, 1,
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG),
                                                 new_unsigned_iliteral(INTEGER_UNSIGNED_LONGLONG, 6ull))))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONGLONG)),
                                           new_string("z")))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2, // non-terminal
                new_dtyped_srt(
                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONGLONG)), 1, // non-terminal
                    new_dtyped_srt(SRT_INDIR_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 1,    // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR,
                                                      new_pointer_dtype(new_integer_dtype(DTYPE_UNSIGNED_LONGLONG)),
                                                      new_string("z")))),
                new_dtyped_srt(
                    SRT_SUB_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("x")),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_UNSIGNED_LONGLONG), new_string("y"))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                             // inst
                              new_mem_immcope(8),                                                          // dst
                              new_unsigned_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG, 8ull), // fst_src
                              NULL));                                                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                                             // inst
                              new_mem_immcope(16),                                                         // dst
                              new_unsigned_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG, 6ull), // fst_src
                              NULL));                                                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                            // fst_src
                              NULL));                                        // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                           // fst_src
                              NULL));                                        // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                                   // inst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(24),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                               // inst
                              new_ptr_immcope(3),                            // dst
                              new_unsigned_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                                        // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_void_pointer(void) {
    Vector* malloc_params = new_vector(&t_dparam);
    vector_push(malloc_params, new_named_dparam(new_string("n"), new_integer_dtype(DTYPE_INT)));
    DType* malloc_dtype = new_function_dtype(malloc_params, new_pointer_dtype(new_void_dtype()));

    Vector* free_params = new_vector(&t_dparam);
    vector_push(free_params, new_named_dparam(new_string("p"), new_pointer_dtype(new_void_dtype())));
    DType* free_dtype = new_function_dtype(free_params, new_void_dtype());

    Srt* input = new_srt(
        SRT_CMPD_STMT, 4,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, malloc_dtype, new_string("malloc")))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, free_dtype, new_string("free")))),
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), new_string("x")),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_dtyped_srt(
                                    SRT_CAST_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                    new_dtyped_srt(
                                        SRT_CALL_EXPR, new_pointer_dtype(new_void_dtype()), 2, // non-terminal
                                        new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(malloc_dtype)),
                                                       1, // non-terminal
                                                       new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(malloc_dtype),
                                                                          new_string("malloc"))),
                                        new_srt(SRT_ARG_LIST, 1, // non-terminal
                                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                 new_signed_iliteral(INTEGER_INT, 4)))))))),
        new_srt(SRT_EXPR_STMT, 1,
                new_dtyped_srt(
                    SRT_CALL_EXPR, new_void_dtype(), 2, // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(dtype_copy(free_dtype)),
                                   1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, dtype_copy(free_dtype), new_string("free"))),
                    new_srt(SRT_ARG_LIST, 1,                                                      // non-terminal
                            new_dtyped_srt(SRT_CAST_EXPR, new_pointer_dtype(new_void_dtype()), 1, // non-terminal
                                           new_identifier_srt(SRT_IDENT_EXPR,
                                                              new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                              new_string("x"))))))

    );

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),                // dst
                              new_label_immcope(new_string("malloc")),                    // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              NULL,                                                       // dst
                              new_label_immcope(new_string("free")),                      // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_compound_stmt_empty(void) {
    Srt* input = new_srt(SRT_CMPD_STMT, 0);

    Vector* expected = new_vector(&t_immc);

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_return_stmt_non_void(void) {
    Srt* input =
        new_srt(SRT_RET_STMT, 1,
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STRET,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src

    run_stmt_immcgen_test(input, NULL, 0, expected);

    delete_vector(expected);
}

void test_immcgen_return_stmt_void(void) {
    Srt* input = new_srt(SRT_RET_STMT, 0);

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src

    run_stmt_immcgen_test(input, NULL, 0, expected);

    delete_vector(expected);
}

void test_immcgen_expression_stmt(void) {
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
                new_inst_immc(IMMC_INST_LOAD,                                            // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),               // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 12), // fst_src
                              NULL));                                                    // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_null_stmt(void) {
    Srt* input = new_srt(SRT_NULL_STMT, 0);

    Vector* expected = new_vector(&t_immc);

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void test_immcgen_if_stmt(void) {
    Srt* input = new_srt(
        SRT_IF_STMT, 2, // non-terminal
        new_dtyped_srt(
            SRT_LESSEQ_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
        new_srt(
            SRT_EXPR_STMT, 1, // non-terminal
            new_dtyped_srt(
                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                               1, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 1)))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JGT,                                              // inst
                              new_label_immcope(new_string(".L0")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_if_else_stmt(void) {
    Srt* input = new_srt(
        SRT_CMPD_STMT, 1, // non-terminal
        new_srt(
            SRT_IF_STMT, 3, // non-terminal
            new_dtyped_srt(
                SRT_EQUAL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
            new_srt(SRT_CMPD_STMT, 1,         // non-terminal
                    new_srt(SRT_EXPR_STMT, 1, // non-terminal
                            new_dtyped_srt(
                                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                new_dtyped_srt(
                                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))),
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 1))))),
            new_srt(SRT_CMPD_STMT, 1,         // non-terminal
                    new_srt(SRT_EXPR_STMT, 1, // non-terminal
                            new_dtyped_srt(
                                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                new_dtyped_srt(
                                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))),
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 2)))))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JNEQ,                                             // inst
                              new_label_immcope(new_string(".L0")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L1")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_if_else_stmt_chain(void) {
    Srt* input = new_srt(
        SRT_IF_STMT, 3, // non-terminal
        new_dtyped_srt(
            SRT_LESS_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
        new_srt(SRT_CMPD_STMT, 1,                                                        // non-terminal
                new_srt(SRT_EXPR_STMT, 1,                                                // non-terminal
                        new_dtyped_srt(SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                       new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      1, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("y"))),
                                       new_dtyped_srt(SRT_NEG_EXPR, new_integer_dtype(DTYPE_INT), 1, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("x")))))),

        new_srt(
            SRT_IF_STMT, 3, // non-terminal
            new_dtyped_srt(
                SRT_GREATER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0))),
            new_srt(SRT_CMPD_STMT, 1,         // non-terminal
                    new_srt(SRT_EXPR_STMT, 1, // non-terminal
                            new_dtyped_srt(
                                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                new_dtyped_srt(
                                    SRT_ADDR_EXPR,
                                    new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))),
                                new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))))),
            new_srt(SRT_CMPD_STMT, 1,         // non-terminal
                    new_srt(SRT_EXPR_STMT, 1, // non-terminal
                            new_dtyped_srt(
                                SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                new_dtyped_srt(
                                    SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("y"))),
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0)))))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JGEQ,                                             // inst
                              new_label_immcope(new_string(".L0")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_NEG,                               // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src

    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    // .L1 is replaced by .L3 (end of if-else chain)
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L3")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JLEQ,                                             // inst
                              new_label_immcope(new_string(".L2")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L3")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L2")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    // .L1 is merged with .L3 (end of if-else chain)
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L3")));

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_while_stmt(void) {
    Srt* input = new_srt(
        SRT_WHILE_STMT, 2, // non-terminal
        new_dtyped_srt(
            SRT_LESS_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
            new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
            new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 10))),
        new_srt(SRT_CMPD_STMT, 2,                                                        // non-terminal
                new_srt(SRT_EXPR_STMT, 1,                                                // non-terminal
                        new_dtyped_srt(SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                       new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      1, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("x"))),
                                       new_dtyped_srt(SRT_ADD_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("x")),
                                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                       new_signed_iliteral(INTEGER_INT, 1))))),
                new_srt(SRT_EXPR_STMT, 1,                                                // non-terminal
                        new_dtyped_srt(SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                       new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)),
                                                      1, // non-terminal
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("y"))),
                                       new_dtyped_srt(SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                                      new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                       new_signed_iliteral(INTEGER_INT, 2)),
                                                      new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                         new_string("y")))))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("y"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JGEQ,                                              // inst
                              new_label_immcope(new_string(".L1")),                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 10))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_for_stmt_init_declaration(void) {
    Srt* input = new_srt(
        SRT_FOR_STMT, 4,                  // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, new_integer_dtype(DTYPE_INT), new_string("i")),
                        new_srt(SRT_INIT, 1, // non-terminal
                                new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                 new_signed_iliteral(INTEGER_INT, 0))))),
        new_srt(SRT_EXPR_STMT, 1,                                              // non-terminal
                new_dtyped_srt(SRT_LESS_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                               new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
                               new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                new_signed_iliteral(INTEGER_INT, 10)))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_POSTINC_EXPR, new_integer_dtype(DTYPE_INT), 1,                                // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i"))))),
        new_srt(
            SRT_CMPD_STMT, 1, // non-terminal
            new_srt(
                SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
                    new_dtyped_srt(SRT_MUL_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                    new_signed_iliteral(INTEGER_INT, 2)))))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(8),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JGEQ,                                              // inst
                              new_label_immcope(new_string(".L1")),                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 10))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_MUL,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_ADD,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_for_stmt_init_expression(void) {
    Srt* input = new_srt(
        SRT_FOR_STMT, 4,          // non-terminal
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i"))),
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("n")))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_GREATER_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i")),
                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT), new_signed_iliteral(INTEGER_INT, 0)))),
        new_srt(SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_POSTDEC_EXPR, new_integer_dtype(DTYPE_INT), 1,                                // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("i"))))),
        new_srt(
            SRT_CMPD_STMT, 1, // non-terminal
            new_srt(
                SRT_EXPR_STMT, 1, // non-terminal
                new_dtyped_srt(
                    SRT_ASSIGN_EXPR, new_integer_dtype(DTYPE_INT), 2,                                 // non-terminal
                    new_dtyped_srt(SRT_ADDR_EXPR, new_pointer_dtype(new_integer_dtype(DTYPE_INT)), 1, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x"))),
                    new_dtyped_srt(SRT_DIV_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                   new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT), new_string("x")),
                                   new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                    new_signed_iliteral(INTEGER_INT, 2)))))));

    SymbolTable* local_table = new_symboltable();
    symboltable_define_memory(local_table, new_string("i"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("n"), new_integer_dtype(DTYPE_INT));
    symboltable_define_memory(local_table, new_string("x"), new_integer_dtype(DTYPE_INT));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JLEQ,                                             // inst
                              new_label_immcope(new_string(".L1")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // dst
                              new_mem_immcope(12),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_DIV,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(12),                         // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));

    run_stmt_immcgen_test(input, local_table, -1, expected);

    delete_vector(expected);
}

void test_immcgen_for_stmt_all_null(void) {
    Srt* input = new_srt(SRT_FOR_STMT, 4, // non-terminal
                         new_srt(SRT_NULL_STMT, 0), new_srt(SRT_NULL_STMT, 0), new_srt(SRT_NULL_STMT, 0),
                         new_srt(SRT_CMPD_STMT, 0));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));

    run_stmt_immcgen_test(input, NULL, -1, expected);

    delete_vector(expected);
}

void run_stmt_immcgen_test(Srt* input, SymbolTable* local_table, int return_label_id, Vector* expected) {
    Immcgen* immcgen = new_immcgen(input);
    if (local_table != NULL) {
        local_table->outer_scope = immcgen->symbol_table;
        immcgen->symbol_table = local_table;
    }
    immcgen->return_label_id = return_label_id;

    Vector* actual = immcgen_generate_immcode(immcgen);

    testlib_assert_immcs_equal(actual, expected);

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_immcgen(immcgen);
}
