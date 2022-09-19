#include "./test_x64gen.h"
#include "../../src/x64gen/x64gen.h"
#include "../testlib/testlib.h"

void test_x64gen_without_evacuation();
void test_x64gen_with_evacuation();

void run_x64gen_test(Vector* __restrict__ input_immcs, Vector* __restrict__ input_liveseqs,
                     Vector* __restrict__ expected);

CU_Suite* add_test_suite_x64gen() {
    CU_Suite* suite = CU_add_suite("test_suite_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_without_evacuation);
    CU_ADD_TEST(suite, test_x64gen_with_evacuation);
    return suite;
}

void test_x64gen_without_evacuation() {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("count")));
    vector_push(input_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("countup")));
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_label_immcope(new_string("count")), // fst_src
                              NULL));                                 // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ADD,                                          // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),                   // fst_src
                              new_signed_immcope(IMMC_SUFFIX_LONG, INTEGER_INT, 1))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                        // inst
                              new_label_immcope(new_string("count")), // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // fst_src
                              NULL));                                 // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JMP,                       // inst
                              new_label_immcope(new_string("L0")), // dst
                              NULL,                                // fst_src
                              NULL));                              // snd_src
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("L0")));
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(4);
    liveness->last_use_index = 5;
    vector_push(liveseq->livenesses, liveness);
    liveness = new_liveness(5);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_VARIABLE, X64_VIS_GLOBAL, new_string("count")));
    vector_push(expected, new_int_data_x64(X64_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_label_x64(X64_LABEL_FUNCTION, X64_VIS_GLOBAL, new_string("countup")));
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                             // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID), // src
                             NULL));                                     // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_label_x64ope(new_string("count")),                      // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                              // inst
                             new_signed_x64ope(X64_SUFFIX_LONG, INTEGER_INT, 1),         // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_label_x64ope(new_string("count"))));                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_JMP,                          // inst
                             NULL,                                  // src
                             new_jlabel_x64ope(new_string("L0")))); // dst
    vector_push(expected, new_label_x64(X64_LABEL_NORMAL, X64_VIS_NONE, new_string("L0")));
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_RET, // inst
                             NULL,         // src
                             NULL));       // dst

    run_x64gen_test(input_immcs, input_liveseqs, expected);
}

void test_x64gen_with_evacuation() {
    Vector* input_immcs = new_vector(&t_immc);
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("count")));
    vector_push(input_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_GLOBAL, new_string("dummy")));
    vector_push(input_immcs, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, new_string("countup")));
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_ENTER,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0),   // dst
                              new_label_immcope(new_string("dummy")), // fst_src
                              NULL));                                 // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LOAD,                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),   // dst
                              new_label_immcope(new_string("count")), // fst_src
                              NULL));                                 // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                       // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STARG,                      // inst
                              new_arg_immcope(IMMC_SUFFIX_LONG, 0), // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1), // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_CALL,                                         // inst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),                   // dst
                              new_label_immcope(new_string("countup_inner")),         // fst_src
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1))); // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_PREP,                                       // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_NONE, INTEGER_INT, 1), // fst_src
                              NULL));                                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STORE,                        // inst
                              new_label_immcope(new_string("count")), // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 1),   // fst_src
                              NULL));                                 // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_STRET,                      // inst
                              NULL,                                 // dst
                              new_reg_immcope(IMMC_SUFFIX_LONG, 0), // fst_src
                              NULL));                               // snd_src
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_JMP,                       // inst
                              new_label_immcope(new_string("L0")), // dst
                              NULL,                                // fst_src
                              NULL));                              // snd_src
    vector_push(input_immcs, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("L0")));
    vector_push(input_immcs,
                new_inst_immc(IMMC_INST_LEAVE,                                      // inst
                              NULL,                                                 // dst
                              new_signed_immcope(IMMC_SUFFIX_QUAD, INTEGER_INT, 0), // fst_src
                              NULL));                                               // snd_src

    Vector* input_liveseqs = new_vector(&t_liveseq);
    vector_fill(input_liveseqs, 8, new_liveseq());

    Liveseq* liveseq = NULL;
    Liveness* liveness = NULL;
    liveseq = vector_at(input_liveseqs, 0);
    liveness = new_liveness(6);
    liveness->last_use_index = 13;
    vector_push(liveseq->livenesses, liveness);
    liveseq = vector_at(input_liveseqs, 1);
    liveness = new_liveness(7);
    liveness->last_use_index = 9;
    liveness = new_liveness(10);
    liveness->last_use_index = 12;
    vector_push(liveseq->livenesses, liveness);

    Vector* expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_VARIABLE, X64_VIS_GLOBAL, new_string("count")));
    vector_push(expected, new_int_data_x64(X64_DATA_LONG, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(expected, new_label_x64(X64_LABEL_VARIABLE, X64_VIS_GLOBAL, new_string("dummy")));
    vector_push(expected, new_int_data_x64(X64_DATA_LONG, new_signed_iliteral(INTEGER_INT, 5)));
    vector_push(expected, new_label_x64(X64_LABEL_FUNCTION, X64_VIS_GLOBAL, new_string("countup")));
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                             // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID), // src
                             NULL));                                     // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_SUBX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_label_x64ope(new_string("dummy")),                      // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_label_x64ope(new_string("count")),                      // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_PUSHX,                                           // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[1]), // src
                             NULL));                                                   // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, DI_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 0), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, AX_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_CALL,                                  // inst
                             new_jlabel_x64ope(new_string("countup_inner")), // src
                             NULL));                                         // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]),   // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLER_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                              // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID),                 // src
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[1]), // src
                             new_label_x64ope(new_string("count"))));                  // dst
    vector_push(expected,
                new_inst_x64(X64_INST_MOVX,                                            // inst
                             new_reg_x64ope(X64_SUFFIX_LONG, CALLER_SAVED_REG_IDS[0]), // src
                             new_reg_x64ope(X64_SUFFIX_LONG, AX_REG_ID)));             // dst
    vector_push(expected,
                new_inst_x64(X64_INST_JMP,                          // inst
                             NULL,                                  // src
                             new_jlabel_x64ope(new_string("L0")))); // dst
    vector_push(expected, new_label_x64(X64_LABEL_NORMAL, X64_VIS_NONE, new_string("L0")));
    vector_push(expected,
                new_inst_x64(X64_INST_ADDX,                                      // inst
                             new_signed_x64ope(X64_SUFFIX_QUAD, INTEGER_INT, 8), // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, SP_REG_ID)));       // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                              // inst
                             NULL,                                                       // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, CALLEE_SAVED_REG_IDS[0]))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_POPX,                                // inst
                             NULL,                                         // src
                             new_reg_x64ope(X64_SUFFIX_QUAD, BP_REG_ID))); // dst
    vector_push(expected,
                new_inst_x64(X64_INST_RET, // inst
                             NULL,         // src
                             NULL));       // dst

    run_x64gen_test(input_immcs, input_liveseqs, expected);
}

void run_x64gen_test(Vector* __restrict__ input_immcs, Vector* __restrict__ input_liveseqs,
                     Vector* __restrict__ expected) {
    X64gen* x64gen = new_x64gen(input_immcs, input_liveseqs);

    Vector* actual = x64gen_generate_x64code(x64gen);

    CU_ASSERT_TRUE(testlib_x64codes_equals(actual, expected));

    delete_vector(actual);
    delete_x64gen(x64gen);
}
