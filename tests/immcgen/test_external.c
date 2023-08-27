#include "./test_external.h"
#include "../../src/immcgen/immcgen.h"
#include "../testlib/testlib.h"

void test_immcgen_transration_unit(void);

void run_immcgen_test(Srt* input, Vector* expected);

CU_Suite* add_test_suite_exteral_immcgen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_exteral_immcgen", NULL, NULL);
    CU_ADD_TEST(suite, test_immcgen_transration_unit);
    return suite;
}

void test_immcgen_transration_unit(void) {
    Vector* incr_params = new_vector(&t_dparam);
    vector_push(incr_params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* incr_dtype = new_function_dtype(incr_params, new_integer_dtype(DTYPE_INT));

    Vector* decr_params = new_vector(&t_dparam);
    vector_push(decr_params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* decr_dtype = new_function_dtype(decr_params, new_integer_dtype(DTYPE_INT));

    Vector* cast_params = new_vector(&t_dparam);
    vector_push(cast_params, new_named_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    DType* cast_dtype = new_function_dtype(cast_params, new_integer_dtype(DTYPE_CHAR));

    Srt* input =
        new_srt(SRT_TRAS_UNIT, 3,                 // non-terminal
                new_srt(SRT_DECL_LIST, 1,         // non-terminal
                        new_srt(SRT_INIT_DECL, 1, // non-terminal
                                new_identifier_srt(SRT_DECL, incr_dtype, new_string("incriment")))),
                new_srt(SRT_FUNC_DEF, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, decr_dtype, new_string("decriment")),
                        new_srt(SRT_CMPD_STMT, 1,                                                     // non-terminal
                                new_srt(SRT_RET_STMT, 1,                                              // non-terminal
                                        new_dtyped_srt(SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                          new_string("x")),
                                                       new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                        new_signed_iliteral(INTEGER_INT, 1)))))),
                new_srt(SRT_FUNC_DEF, 2, // non-terminal
                        new_identifier_srt(SRT_DECL, cast_dtype, new_string("cast")),
                        new_srt(SRT_CMPD_STMT, 1,                                                       // non-terminal
                                new_srt(SRT_RET_STMT, 1,                                                // non-terminal
                                        new_dtyped_srt(SRT_CAST_EXPR, new_integer_dtype(DTYPE_CHAR), 1, // non-terminal
                                                       new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                                          new_string("x")))))));

    Vector* expected = new_vector(&t_immc);
    vector_push(expected, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("decriment")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_SUB,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("cast")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 1), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L1")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));
    vector_push(expected,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src

    run_immcgen_test(input, expected);

    delete_vector(expected);
}

void run_immcgen_test(Srt* input, Vector* expected) {
    Immcgen* immcgen = new_immcgen(input);

    Vector* actual = immcgen_generate_immcode(immcgen);

    testlib_assert_immcs_equal(actual, expected);

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_immcgen(immcgen);
}
