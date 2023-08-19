#include "./test_datainst.h"
#include "../../src/x64gen/inst.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_x64gen_load_int(void);
void test_x64gen_load_reg(void);
void test_x64gen_load_ptr(void);
void test_x64gen_load_mem(void);
void test_x64gen_load_label(void);
void test_x64gen_addr_mem(void);
void test_x64gen_addr_label(void);
void test_x64gen_str_byte(void);
void test_x64gen_str_word(void);
void test_x64gen_str_long(void);
void test_x64gen_str_quad(void);
void test_x64gen_str_other(void);
void test_x64gen_store_int_ptr(void);
void test_x64gen_store_int_mem(void);
void test_x64gen_store_int_label(void);
void test_x64gen_store_reg_ptr(void);
void test_x64gen_store_reg_mem(void);
void test_x64gen_store_reg_label(void);
void test_x64gen_ldarg_sixth_or_less(void);
void test_x64gen_ldarg_seventh_or_more(void);
void test_x64gen_starg_int(void);
void test_x64gen_starg_reg(void);
void test_x64gen_stret_int(void);
void test_x64gen_stret_reg(void);

void run_datainst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected);

CU_Suite* add_test_suite_datainst_x64gen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_datainst_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_load_int);
    CU_ADD_TEST(suite, test_x64gen_load_reg);
    CU_ADD_TEST(suite, test_x64gen_load_ptr);
    CU_ADD_TEST(suite, test_x64gen_load_mem);
    CU_ADD_TEST(suite, test_x64gen_load_label);
    CU_ADD_TEST(suite, test_x64gen_addr_mem);
    CU_ADD_TEST(suite, test_x64gen_addr_label);
    CU_ADD_TEST(suite, test_x64gen_str_byte);
    CU_ADD_TEST(suite, test_x64gen_str_word);
    CU_ADD_TEST(suite, test_x64gen_str_long);
    CU_ADD_TEST(suite, test_x64gen_str_quad);
    CU_ADD_TEST(suite, test_x64gen_str_other);
    CU_ADD_TEST(suite, test_x64gen_store_int_ptr);
    CU_ADD_TEST(suite, test_x64gen_store_int_mem);
    CU_ADD_TEST(suite, test_x64gen_store_int_label);
    CU_ADD_TEST(suite, test_x64gen_store_reg_ptr);
    CU_ADD_TEST(suite, test_x64gen_store_reg_mem);
    CU_ADD_TEST(suite, test_x64gen_store_reg_label);
    CU_ADD_TEST(suite, test_x64gen_ldarg_sixth_or_less);
    CU_ADD_TEST(suite, test_x64gen_ldarg_seventh_or_more);
    CU_ADD_TEST(suite, test_x64gen_starg_int);
    CU_ADD_TEST(suite, test_x64gen_starg_reg);
    CU_ADD_TEST(suite, test_x64gen_stret_int);
    CU_ADD_TEST(suite, test_x64gen_stret_reg);
    return suite;
}

void test_x64gen_load_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                                           // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 5), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 5),     // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_load_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_load_ptr(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 1), // dst
                              new_ptr_immcope(0),                          // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_ptr_x64ope(CALLER_SAVED_REG_IDS[0]),                    // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_load_mem(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ADDR,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_LEAX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_load_label(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ADDR,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // dst
                              new_label_immcope(new_string("variable")),   // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_LEAX,                                              // inst
                             new_label_x64ope(new_string("variable")),                   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_addr_mem(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_mem_immcope(4),                          // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_mem_x64ope(4),                                          // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_addr_label(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                              // inst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_label_immcope(new_string("variable")),   // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_label_x64ope(new_string("variable")),                   // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_str_byte(void) {
    char sliteral_const[1] = "";
    int sliteral_size = 1;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STR,                  // inst
                              new_mem_immcope(sliteral_size), // dst
                              new_str_immcope(sliteral),      // fst_src
                              NULL));                         // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                                          // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_BYTE, INTEGER_UNSIGNED_LONGLONG, 0), // src
                             new_mem_x64ope(1)));                                                    // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_str_word(void) {
    char sliteral_const[10] = "123456789";
    int sliteral_size = 10;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STR,                  // inst
                              new_mem_immcope(sliteral_size), // dst
                              new_str_immcope(sliteral),      // fst_src
                              NULL));                         // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVABSX, // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG,
                                                     0x3837363534333231u),  // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(10)));                        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                                              // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_WORD, INTEGER_UNSIGNED_LONGLONG, 0x39u), // src
                             new_mem_x64ope(2)));                                                        // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_str_long(void) {
    char sliteral_const[12] = "ABCDEFGHIJK";
    int sliteral_size = 12;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STR,                  // inst
                              new_mem_immcope(sliteral_size), // dst
                              new_str_immcope(sliteral),      // fst_src
                              NULL));                         // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVABSX, // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG,
                                                     0x4847464544434241u),  // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(12)));                        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                                                  // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_LONG, INTEGER_UNSIGNED_LONGLONG, 0x4B4A49u), // src
                             new_mem_x64ope(4)));                                                            // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_str_quad(void) {
    char sliteral_const[16] = "ABCDEFGHIJKLMNO";
    int sliteral_size = 16;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STR,                  // inst
                              new_mem_immcope(sliteral_size), // dst
                              new_str_immcope(sliteral),      // fst_src
                              NULL));                         // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVABSX, // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG,
                                                     0x4847464544434241u),  // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(16)));                        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVABSX, // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG,
                                                     0x4F4E4D4C4B4A49u),    // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(8)));                         // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_str_other(void) {
    char sliteral_const[11] = "ABCDEFGHIJ";
    int sliteral_size = 11;
    char* sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* sliteral = new_sliteral(sliteral_value, sliteral_size);

    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STR,                  // inst
                              new_mem_immcope(sliteral_size), // dst
                              new_str_immcope(sliteral),      // fst_src
                              NULL));                         // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVABSX, // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG,
                                                     0x4847464544434241u),  // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(11)));                        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVABSX, // inst
                             new_unsigned_int_x64ope(X64_SUFFIX_QUAD, INTEGER_UNSIGNED_LONGLONG,
                                                     0x4A494847464544u),    // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(8)));                         // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_store_int_ptr(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_ptr_immcope(0),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 8), // src
                             new_ptr_x64ope(CALLER_SAVED_REG_IDS[0])));              // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_store_int_mem(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_mem_immcope(4),                                       // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 8), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 8), // src
                             new_mem_x64ope(4)));                                    // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_store_int_label(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                                          // inst
                              new_label_immcope(new_string("variable")),                // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 4), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 4), // src
                             new_label_x64ope(new_string("variable"))));             // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_store_reg_ptr(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_ptr_immcope(1),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_ptr_x64ope(CALLER_SAVED_REG_IDS[1])));                // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_store_reg_mem(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_mem_immcope(1),                          // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_BYTE, CALLER_SAVED_REG_IDS[0]), // src
                             new_mem_x64ope(1)));                                      // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_store_reg_label(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                             // inst
                              new_label_immcope(new_string("variable")),   // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_label_x64ope(new_string("variable"))));               // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_ldarg_sixth_or_less(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(24),                         // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 5), // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(20),                         // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 4), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                   // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, ARG_REG_IDS[5]), // src
                             new_mem_x64ope(24)));                            // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                   // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, ARG_REG_IDS[4]), // src
                             new_mem_x64ope(20)));                            // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_ldarg_seventh_or_more(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(32),                         // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_QUAD, 6), // fst_src
                              NULL));                                      // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LDARG,                             // inst
                              new_mem_immcope(36),                         // dst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 7), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                 // inst
                             new_mem_x64ope(-16),                           // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, R10_REG_ID), // src
                             new_mem_x64ope(32)));                        // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                 // inst
                             new_mem_x64ope(-24),                           // src
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                               // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, R10_REG_ID), // src
                             new_mem_x64ope(36)));                        // dst

    //     Stack Frame Layout
    // ==========================
    //  caller's old %rbp
    // --------------------------
    //  caller's local variables
    //  (aligned on a 16 bytes)
    // --------------------------
    //  ..... (more arguments)
    // --------------------------
    //  8th argument (or blank)
    // -------------------------- <- -16(%rbp)
    //  7th argument
    // ==========================
    //  old %rbp
    // -------------------------- <- (%rbp)
    //  local variables
    //  (aligned on a 16 bytes)
    // --------------------------

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_starg_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                                          // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_LONG, 0),              // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 6), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                         // inst
                             new_signed_int_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 6), // src
                             NULL));                                                 // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_starg_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                             // inst
                              new_signed_arg_immcope(IMMC_SUFFIX_BYTE, 0), // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_BYTE, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_stret_int(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STRET,                                          // inst
                              NULL,                                                     // dst
                              new_signed_int_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 5), // fst_src
                              NULL));                                                   // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                          // inst
                             new_signed_int_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 5), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));           // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void test_x64gen_stret_reg(void) {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STRET,                             // inst
                              NULL,                                        // dst
                              new_signed_reg_immcope(IMMC_SUFFIX_QUAD, 0), // fst_src
                              NULL));                                      // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));             // dst

    run_datainst_x64gen_test(input_immcs, input_liveseqs, expected);

    delete_vector(expected);
}

void run_datainst_x64gen_test(Vector* input_immcs, Vector* input_liveseqs, Vector* expected) {
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
