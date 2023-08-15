#include "./test_arithinst.h"
#include "../../src/x64gen/inst.h"
#include "../testlib/testlib.h"

void test_x64gen_add_reg_int(void);
void test_x64gen_add_reg_reg(void);
void test_x64gen_sub_reg_int(void);
void test_x64gen_sub_reg_reg(void);
void test_x64gen_mul_reg_int(void);
void test_x64gen_mul_reg_reg(void);
void test_x64gen_umul_reg_int(void);
void test_x64gen_umul_reg_reg(void);
void test_x64gen_div_reg_int(void);
void test_x64gen_div_reg_reg(void);
void test_x64gen_div_rdx_inarg(void);
void test_x64gen_div_rdx_inuse(void);
void test_x64gen_udiv_reg_int(void);
void test_x64gen_udiv_reg_reg(void);
void test_x64gen_udiv_rdx_inarg(void);
void test_x64gen_udiv_rdx_inuse(void);
void test_x64gen_mod_reg_int(void);
void test_x64gen_mod_reg_reg(void);
void test_x64gen_mod_rdx_inarg(void);
void test_x64gen_mod_rdx_inuse(void);
void test_x64gen_umod_reg_int(void);
void test_x64gen_umod_reg_reg(void);
void test_x64gen_umod_rdx_inarg(void);
void test_x64gen_umod_rdx_inuse(void);

void run_arithinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected);

CU_Suite* add_test_suite_arithinst_x64gen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_arithinst_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_add_reg_int);
    CU_ADD_TEST(suite, test_x64gen_add_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_sub_reg_int);
    CU_ADD_TEST(suite, test_x64gen_sub_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_mul_reg_int);
    CU_ADD_TEST(suite, test_x64gen_mul_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_umul_reg_int);
    CU_ADD_TEST(suite, test_x64gen_umul_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_div_reg_int);
    CU_ADD_TEST(suite, test_x64gen_div_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_div_rdx_inarg);
    CU_ADD_TEST(suite, test_x64gen_div_rdx_inuse);
    CU_ADD_TEST(suite, test_x64gen_udiv_reg_int);
    CU_ADD_TEST(suite, test_x64gen_udiv_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_udiv_rdx_inarg);
    CU_ADD_TEST(suite, test_x64gen_udiv_rdx_inuse);
    CU_ADD_TEST(suite, test_x64gen_mod_reg_int);
    CU_ADD_TEST(suite, test_x64gen_mod_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_mod_rdx_inarg);
    CU_ADD_TEST(suite, test_x64gen_mod_rdx_inuse);
    CU_ADD_TEST(suite, test_x64gen_umod_reg_int);
    CU_ADD_TEST(suite, test_x64gen_umod_reg_reg);
    CU_ADD_TEST(suite, test_x64gen_umod_rdx_inarg);
    CU_ADD_TEST(suite, test_x64gen_umod_rdx_inuse);
    return suite;
}

void test_x64gen_add_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ADD,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

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
                new_inst_x64(X64_INST_ADDX,                                              // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2),         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_add_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(12),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ADD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_sub_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_SUB,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

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
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_SUBX,                                              // inst
                             new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 2),         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_sub_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 1), // dst
                              new_mem_immcope(2),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_SUB,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 1))); // snd_src

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
                             new_mem_x64ope(1),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(2),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_SUBX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_mul_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MUL,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

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
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IMULX,                                     // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_mul_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(5),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MUL,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
                             new_mem_x64ope(1),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(5),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                          // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IMULX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_umul_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UMUL,                                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

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
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IMULX,                                     // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_umul_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MUL,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IMULX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_div_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_DIV,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

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
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID)));      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CXTD,                      // inst
                             new_suffix_x64ope(X64_SUFFIX_LONG), // src
                             NULL));                             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             NULL));                                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_div_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(12),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_DIV,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(12),                                         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CQTO, // inst
                             NULL,          // src
                             NULL));        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_div_rdx_inarg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_DIV,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CXTD,                      // inst
                             new_suffix_x64ope(X64_SUFFIX_LONG), // src
                             NULL));                             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_div_rdx_inuse(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_DIV,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);

    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    // lock DX_REG

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_mem_x64ope(8),                            // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID)));      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CXTD,                      // inst
                             new_suffix_x64ope(X64_SUFFIX_LONG), // src
                             NULL));                             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             NULL));                                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID)));             // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_udiv_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UDIV,                                                     // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                               // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                               // fst_src
                              new_unsigned_immcope(IMMC_SUFFIX_LONG, INTEGER_UNSIGNED_INT, 3u))); // snd_src

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
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                                  // inst
                             new_unsigned_x64ope(X64_SUFFIX_LONG, INTEGER_UNSIGNED_INT, 3u), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID)));                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             NULL));                                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_udiv_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(12),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UDIV,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(12),                                         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_udiv_rdx_inarg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UDIV,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_udiv_rdx_inuse(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UDIV,                                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 2))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);

    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    // lock DX_REG

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_mem_x64ope(8),                            // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 2), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID)));      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             NULL));                                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID)));             // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_mod_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MOD,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 3))); // snd_src

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
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 3), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID)));      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CXTD,                      // inst
                             new_suffix_x64ope(X64_SUFFIX_LONG), // src
                             NULL));                             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             NULL));                                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_mod_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(12),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MOD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(12),                                         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CQTO, // inst
                             NULL,          // src
                             NULL));        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_mod_rdx_inarg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MOD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CXTD,                      // inst
                             new_suffix_x64ope(X64_SUFFIX_LONG), // src
                             NULL));                             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[2]), // src
                             NULL));                                                   // dst

    // movx instruction is omitted,
    // since CALLER_SAVED_REG_IDS[1] is DX_REG_ID, which has modulo result of idivx instruction

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_mod_rdx_inuse(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(12),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(16),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_MOD,                          // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    // lock DX_REG

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_mem_x64ope(4),                            // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(12),                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(16),                                         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CQTO, // inst
                             NULL,          // src
                             NULL));        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_IDIVX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID)));             // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_umod_reg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UMOD,                                                     // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                               // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                               // fst_src
                              new_unsigned_immcope(IMMC_SUFFIX_LONG, INTEGER_UNSIGNED_INT, 4u))); // snd_src

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
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                                  // inst
                             new_unsigned_x64ope(X64_SUFFIX_LONG, INTEGER_UNSIGNED_INT, 4u), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID)));                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             NULL));                                      // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_umod_reg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 3), // dst
                              new_mem_immcope(9),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UMOD,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_QUAD, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 3))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(9),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[3]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[3]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[2]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_umod_rdx_inarg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_mem_immcope(1),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(5),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UMOD,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_reg_immcope(IMMC_SUFFIX_BYTE, 0),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1))); // snd_src

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
                             new_mem_x64ope(1),                                          // src
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(5),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVSXX,                                          // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_umod_rdx_inuse(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_mem_immcope(4),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2), // dst
                              new_mem_immcope(8),                   // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                       // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3), // dst
                              new_mem_immcope(12),                  // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_UMOD,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 2),   // fst_src
                              new_reg_immcope(IMMC_SUFFIX_LONG, 3))); // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 2);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 3);
    liveness = new_liveness(1);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    liveness->last_use_index = 4;
    vector_push(liveseq->livenesses, liveness);
    // lock DX_REG

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_mem_x64ope(4),                            // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(8),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(12),                                         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[2]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_DIVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[3]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, DX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[2]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DX_REG_ID)));             // dst

    run_arithinst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void run_arithinst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected) {
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
