#include "./test_label.h"
#include "../../src/x64gen/label.h"
#include "../testlib/testlib.h"

void test_x64gen_label(void);

void run_label_x64gen_test(Vector* input, Vector* expected);

CU_Suite* add_test_suite_label_x64gen(void) {
    CU_Suite* suite = CU_add_suite("test_suite_label_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_label);
    return suite;
}

void test_x64gen_label(void) {
    Vector* input = NULL;
    Vector* expected = NULL;

    input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, new_string("normal_none_label")));
    expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_NORMAL, X64_VIS_NONE, new_string("normal_none_label")));
    run_label_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_NONE, new_string("function_label")));
    expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_FUNCTION, X64_VIS_NONE, new_string("function_label")));
    run_label_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_VARIABLE, IMMC_VIS_NONE, new_string("variable_label")));
    expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_VARIABLE, X64_VIS_NONE, new_string("variable_label")));
    run_label_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_GLOBAL, new_string("global_label")));
    expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_NORMAL, X64_VIS_GLOBAL, new_string("global_label")));
    run_label_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_LOCAL, new_string("local_label")));
    expected = new_vector(&t_x64);
    vector_push(expected, new_label_x64(X64_LABEL_NORMAL, X64_VIS_LOCAL, new_string("local_label")));
    run_label_x64gen_test(input, expected);
    delete_vector(expected);
}

void run_label_x64gen_test(Vector* input, Vector* expected) {
    Vector* liveseqs = new_vector(&t_liveseq);
    vector_fill(liveseqs, 8, new_liveseq());
    X64gen* x64gen = new_x64gen(input, liveseqs);

    Vector* actual = gen_label_x64code(x64gen);

    testlib_assert_x64codes_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_x64gen(x64gen);
}
