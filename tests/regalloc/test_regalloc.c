#include "./test_regalloc.h"
#include "../../src/regalloc/regalloc.h"
#include "../testlib/testlib.h"

void test_regalloc_empty_function(void);
void test_regalloc_single_register(void);
void test_regalloc_multiple_register(void);
void test_regalloc_longlife_register(void);
void test_regalloc_jump(void);
void test_regalloc_loop(void);
void test_regalloc_global_variable(void);
void test_regalloc_function_variable_mixed(void);

void run_regalloc_test(Vector* input, int num_regs, Vector* expected_immcs, Vector* expected_liveseqs);

CU_Suite* add_test_suite_regalloc(void) {
    CU_Suite* suite = CU_add_suite("test_suite_regalloc", NULL, NULL);
    CU_ADD_TEST(suite, test_regalloc_empty_function);
    CU_ADD_TEST(suite, test_regalloc_single_register);
    CU_ADD_TEST(suite, test_regalloc_multiple_register);
    CU_ADD_TEST(suite, test_regalloc_longlife_register);
    CU_ADD_TEST(suite, test_regalloc_jump);
    CU_ADD_TEST(suite, test_regalloc_loop);
    CU_ADD_TEST(suite, test_regalloc_global_variable);
    CU_ADD_TEST(suite, test_regalloc_function_variable_mixed);
    return suite;
}

void test_regalloc_empty_function(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("empty")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("empty")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_single_register(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("decriment")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_SUB,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("decriment")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_SUB,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(expected_liveseqs, 0);
    liveness = new_liveness(3);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(4);
    liveness->last_use_index = 5;
    vector_push(liveseq->livenesses, liveness);

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_multiple_register(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("add")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_ADD,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("add")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ADD,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(expected_liveseqs, 0);
    liveness = new_liveness(4);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(6);
    liveness->last_use_index = 7;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(expected_liveseqs, 1);
    liveness = new_liveness(5);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_longlife_register(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("update_ptr")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(1),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(2),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STRET,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("update_ptr")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(1),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(1),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STRET,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(expected_liveseqs, 0);
    liveness = new_liveness(2);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(expected_liveseqs, 1);
    liveness = new_liveness(3);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(5);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_jump(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("check_validate")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_SETNEQ,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JEQ,                                              // inst
                              new_label_immcope(new_string(".L1")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_ADDR,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(12),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),                // dst
                              new_label_immcope(new_string("validate")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JEQ,                                              // inst
                              new_label_immcope(new_string(".L1")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_sr
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L2")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_sr
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L2")));
    vector_push(input,
                new_inst_immc(IMMC_INST_STRET,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("check_validate")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_SETNEQ,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JEQ,                                              // inst
                              new_label_immcope(new_string(".L1")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ADDR,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(12),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_label_immcope(new_string("validate")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JEQ,                                              // inst
                              new_label_immcope(new_string(".L1")),                       // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_sr
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L2")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_sr
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L2")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STRET,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(expected_liveseqs, 0);
    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(3);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(6);
    liveness->last_use_index = 7;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(8);
    liveness->last_use_index = 10;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(11);
    vector_push(liveseq->livenesses, liveness);

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_loop(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("loop")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JGEQ,                                              // inst
                              new_label_immcope(new_string(".L2")),                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 10))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_ADD,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 2), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_MUL,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 3),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 4))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 5), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L1")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L2")));
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("loop")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L1")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JGEQ,                                              // inst
                              new_label_immcope(new_string(".L2")),                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                 // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 10))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ADD,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(4),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_MUL,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(8),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L1")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L2")));
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(expected_liveseqs, 0);
    liveness = new_liveness(3);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(5);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(6);
    liveness->last_use_index = 7;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(8);
    liveness->last_use_index = 10;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(10);
    liveness->last_use_index = 11;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(expected_liveseqs, 1);
    liveness = new_liveness(9);
    liveness->last_use_index = 10;
    vector_push(liveseq->livenesses, liveness);

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_global_variable(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("a")));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(input, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("b")));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 6)));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("a")));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 2)));
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("b")));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 6)));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 7)));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 8)));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 0)));

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void test_regalloc_function_variable_mixed(void) {
    Vector* input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("count")));
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("countup")));
    vector_push(input,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_label_immcope(new_string("count")),      // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_ADD,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_label_immcope(new_string("count")),      // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                                      // snd_src
    vector_push(input,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(input,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    int num_regs = 8;

    Vector* expected_immcs = new_vector(&t_immc);
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("count")));
    vector_push(expected_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("countup")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_label_immcope(new_string("count")),      // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_ADD,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_label_immcope(new_string("count")),      // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_JMP,                        // inst
                              new_label_immcope(new_string(".L0")), // dst
                              NULL,                                 // fst_src
                              NULL));                               // snd_src
    vector_push(expected_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string(".L0")));
    vector_push(expected_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    Vector* expected_liveseqs = new_vector(&t_liveseq);
    vector_fill(expected_liveseqs, num_regs, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(expected_liveseqs, 0);
    liveness = new_liveness(4);
    liveness->last_use_index = 5;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(5);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    run_regalloc_test(input, num_regs, expected_immcs, expected_liveseqs);

    delete_vector(expected_immcs);
    delete_vector(expected_liveseqs);
}

void run_regalloc_test(Vector* input, int num_regs, Vector* expected_immcs, Vector* expected_liveseqs) {
    RegAlloc* regalloc = new_regalloc(input, num_regs);
    Vector* actual_immcs = NULL;
    Vector* actual_liveseqs = NULL;
    regallocret_assign(&actual_immcs, &actual_liveseqs, regalloc_allocate_regs(regalloc));

    testlib_assert_immcs_equal(actual_immcs, expected_immcs);
    testlib_assert_liveseqs_equal(actual_liveseqs, expected_liveseqs);

    delete_vector(actual_immcs);
    delete_vector(actual_liveseqs);
    delete_regalloc(regalloc);
}
