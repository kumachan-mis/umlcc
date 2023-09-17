#include "./test_logicinst.h"
#include "../../src/x64gen/inst.h"
#include "../testlib/testlib.h"

void test_x64gen_and_reg_int(void);
void test_x64gen_and_reg_reg(void);
void test_x64gen_not_int(void);
void test_x64gen_not_reg(void);
void test_x64gen_or_reg_int(void);
void test_x64gen_or_reg_reg(void);
void test_x64gen_xor_reg_int(void);
void test_x64gen_xor_reg_reg(void);

void run_logicinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected);

CU_Suite* add_test_suite_logicinst_x64gen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_logicinst_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_and_reg_int);
    CU_ADD_TEST(suite, test_x64gen_and_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_not_int);
    CU_ADD_TEST(suite, test_x64gen_not_reg);
    CU_ADD_TEST(suite, test_x64gen_or_reg_int);
    CU_ADD_TEST(suite, test_x64gen_or_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_xor_reg_int);
    CU_ADD_TEST(suite, test_x64gen_xor_reg_reg);
    return suite;
}

void test_x64gen_and_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_AND,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 1;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ANDX,                                              // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 1),     // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_and_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(12),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_AND,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(12),                                         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CXTX,                        // inst
                             new_suffix_x64ope(X64_SUFFIX_LONG),   // src
                             new_suffix_x64ope(X64_SUFFIX_QUAD))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ANDX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_not_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_NOT,                                            // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1), // fst_src
                              NULL));                                                   // snd_src

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
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 1),     // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_NOTX,                                              // inst
                             NULL,                                                       // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_not_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_NOT,                               // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());
    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 1;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(1);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_NOTX,                                              // inst
                             NULL,                                                       // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_or_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_OR,                                               // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 1;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ORX,                                               // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 1),     // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_or_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1), // dst
                              new_mem_immcope(16),                         // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_OR,                                  // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(16),                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ORX,                                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_xor_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_XOR,                                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),                // fst_src
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 1;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_XORX,                                              // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 1),     // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_xor_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                          // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_XOR,                                 // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0),   // fst_src
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_XORX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_logicinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void run_logicinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected) {
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
