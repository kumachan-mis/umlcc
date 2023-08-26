#include "./test_ctrlinst.h"
#include "../../src/x64gen/inst.h"
#include "../testlib/testlib.h"

void test_x64gen_jmp(void);
void test_x64gen_jeq_reg_int(void);
void test_x64gen_jeq_reg_reg(void);
void test_x64gen_jneq_reg_int(void);
void test_x64gen_jneq_reg_reg(void);
void test_x64gen_call_label_six_or_less(void);
void test_x64gen_call_label_seven_or_more_odd(void);
void test_x64gen_call_label_seven_or_more_even(void);
void test_x64gen_call_ptr_without_evacuation(void);
void test_x64gen_call_ptr_with_evacuation(void);
void test_x64gen_call_caller_saved_evacuation(void);
void test_x64gen_call_no_return_value(void);
void test_x64gen_enter_zero(void);
void test_x64gen_enter_non_16bytes_boundary(void);
void test_x64gen_enter_16bytes_boundary(void);
void test_x64gen_leave_zero(void);
void test_x64gen_leave_non_16bytes_boundary(void);
void test_x64gen_leave_16bytes_boundary(void);

void run_ctrlinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected);

CU_Suite* add_test_suite_ctrlinst_x64gen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_ctrlinst_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_jmp);
    CU_ADD_TEST(suite, test_x64gen_jeq_reg_int);
    CU_ADD_TEST(suite, test_x64gen_jeq_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_jneq_reg_int);
    CU_ADD_TEST(suite, test_x64gen_jneq_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_call_label_six_or_less);
    CU_ADD_TEST(suite, test_x64gen_call_label_seven_or_more_odd);
    CU_ADD_TEST(suite, test_x64gen_call_label_seven_or_more_even);
    CU_ADD_TEST(suite, test_x64gen_call_ptr_without_evacuation);
    CU_ADD_TEST(suite, test_x64gen_call_ptr_with_evacuation);
    CU_ADD_TEST(suite, test_x64gen_call_caller_saved_evacuation);
    CU_ADD_TEST(suite, test_x64gen_call_no_return_value);
    CU_ADD_TEST(suite, test_x64gen_enter_zero);
    CU_ADD_TEST(suite, test_x64gen_enter_non_16bytes_boundary);
    CU_ADD_TEST(suite, test_x64gen_enter_16bytes_boundary);
    CU_ADD_TEST(suite, test_x64gen_leave_zero);
    CU_ADD_TEST(suite, test_x64gen_leave_non_16bytes_boundary);
    CU_ADD_TEST(suite, test_x64gen_leave_16bytes_boundary);
    return suite;
}

void test_x64gen_jmp(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JMP,                          // inst
                              new_label_immcope(new_string("LABEL")), // dst
                              NULL,                                   // fst_src
                              NULL));                                 // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_JMP,                             // inst
                             NULL,                                     // src
                             new_jlabel_x64ope(new_string("LABEL")))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_jeq_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JEQ,                                              // inst
                              new_label_immcope(new_string("LABEL")),                     // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 1;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(1),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CMPX,                                              // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 4),     // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_JE,                              // inst
                             NULL,                                     // src
                             new_jlabel_x64ope(new_string("LABEL")))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_jeq_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JEQ,                                 // inst
                              new_label_immcope(new_string("LABEL")),        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CMPX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_JE,                              // inst
                             NULL,                                     // src
                             new_jlabel_x64ope(new_string("LABEL")))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_jneq_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JNEQ,                                             // inst
                              new_label_immcope(new_string("LABEL")),                     // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 1;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CMPX,                                              // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8),     // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_JNE,                             // inst
                             NULL,                                     // src
                             new_jlabel_x64ope(new_string("LABEL")))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_jneq_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 1), // dst
                              new_mem_immcope(5),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JNEQ,                                // inst
                              new_label_immcope(new_string("LABEL")),        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(5),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CMPX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_JNE,                             // inst
                             NULL,                                     // src
                             new_jlabel_x64ope(new_string("LABEL")))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_label_six_or_less(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 1),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_label_immcope(new_string("function")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(3);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(5);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);

    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 3), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(1),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                             // inst
                             new_jlabel_x64ope(new_string("function")), // src
                             NULL));                                    // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_label_seven_or_more_odd(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 7), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                           // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 6),               // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 12), // fst_src
                              NULL));                                                    // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                           // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 5),               // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 10), // fst_src
                              NULL));                                                    // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 4),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 3),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 6), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 2),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 1),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_label_immcope(new_string("function")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 7))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 7), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(8);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_SUBX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 12), // src
                             NULL));                                                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 10), // src
                             NULL));                                                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 6), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 4), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 2), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R8_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R9_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                             // inst
                             new_jlabel_x64ope(new_string("function")), // src
                             NULL));                                    // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                           // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 16), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));            // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_label_seven_or_more_even(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                           // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 7),               // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 14), // fst_src
                              NULL));                                                    // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                           // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 6),               // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 12), // fst_src
                              NULL));                                                    // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                           // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 5),               // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 10), // fst_src
                              NULL));                                                    // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 4),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 3),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 6), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 2),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 1),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_label_immcope(new_string("function")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 8))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(8);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 14), // src
                             NULL));                                                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 12), // src
                             NULL));                                                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 10), // src
                             NULL));                                                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 6), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 4), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 2), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R8_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R9_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                             // inst
                             new_jlabel_x64ope(new_string("function")), // src
                             NULL));                                    // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                           // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 16), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));            // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_ptr_without_evacuation(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 5), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_ptr_immcope(0),                                         // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(3);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 5), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                            // inst
                             new_jptr_x64ope(CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_ptr_with_evacuation(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 3), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 5), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_ptr_immcope(3),                                         // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(0);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(3);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 5), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID)));            // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,               // inst
                             new_jptr_x64ope(R10_REG_ID), // src
                             NULL));                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    // value in CALLER_SAVED_REG_IDS[3] is evacuated to R10_REG_ID,
    // since CALLER_SAVED_REG_IDS[3] is DI_REG_ID,
    // which is overwritten by popq instruction to set 1st argument of the function.

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_caller_saved_evacuation(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 1),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3), // fst_src
                              NULL));                                                   // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1),                // dst
                              new_label_immcope(new_string("function")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 2), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(4);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(0);
    liveness->last_use_index = 7;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 3), // src
                             NULL));                                                 // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                             // inst
                             new_jlabel_x64ope(new_string("function")), // src
                             NULL));                                    // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_call_no_return_value(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                           // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL)); // snd_src                                       // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                             // inst
                              NULL,                                                       // dst
                              new_label_immcope(new_string("function")),                  // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CLEAN,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));           // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                             // inst
                             new_jlabel_x64ope(new_string("function")), // src
                             NULL));                                    // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_enter_zero(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                             // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID), // src
                             NULL));                                     // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_enter_non_16bytes_boundary(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                           // inst
                              NULL,                                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 31), // fst_src
                              NULL));                                                    // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                             // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID), // src
                             NULL));                                     // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_SUBX,                                           // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 32), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));            // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_enter_16bytes_boundary(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                           // inst
                              NULL,                                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 16), // fst_src
                              NULL));                                                    // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                             // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID), // src
                             NULL));                                     // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_SUBX,                                           // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 16), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));            // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_leave_zero(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_RET, // inst
                             NULL,         // src
                             NULL));       // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_leave_non_16bytes_boundary(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                           // inst
                              NULL,                                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 31), // fst_src
                              NULL));                                                    // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                           // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 32), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));            // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_RET, // inst
                             NULL,         // src
                             NULL));       // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_leave_16bytes_boundary(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                           // inst
                              NULL,                                                      // dst
                              new_signed_int_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 16), // fst_src
                              NULL));                                                    // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                           // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 16), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));            // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_RET, // inst
                             NULL,         // src
                             NULL));       // dst

    run_ctrlinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void run_ctrlinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected) {
    X64gen* x64gen = new_x64gen(input_immcs, input_liveseqs);

    Vector* actual = new_vector(&t_x64);
    int immcs_len = vector_size(input_immcs);
    for (int i = 0; i < immcs_len; i++) {
        Vector* actual_sub = gen_inst_x64code(x64gen);
        vector_extend(actual, actual_sub);
        delete_vector(actual_sub);
    }

    testlib_assert_x64codes_equal(actual, expected);

    if (actual != NULL) {
        delete_vector(actual);
    }
    delete_x64gen(x64gen);
}
