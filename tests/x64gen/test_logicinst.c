#include "./test_logicinst.h"
#include "../../src/x64gen/inst.h"
#include "../testlib/testlib.h"

void test_x64gen_not_int(void);
void test_x64gen_not_reg(void);

void run_logicinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected);

CU_Suite* add_test_suite_logicinst_x64gen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_logicinst_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_not_int);
    CU_ADD_TEST(suite, test_x64gen_not_reg);
    return suite;
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
