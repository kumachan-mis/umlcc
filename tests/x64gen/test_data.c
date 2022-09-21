#include "./test_data.h"
#include "../../src/liveseq/liveseq.h"
#include "../../src/x64gen/data.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_x64gen_data_int();
void test_x64gen_data_str();

void run_data_x64gen_test(Vector* input, Vector* expected);

CU_Suite* add_test_suite_data_x64gen() {
    CU_Suite* suite = CU_add_suite("test_suite_data_x64gen", NULL, NULL);
    CU_ADD_TEST(suite, test_x64gen_data_int);
    CU_ADD_TEST(suite, test_x64gen_data_str);
    return suite;
}

void test_x64gen_data_int() {
    Vector* input = NULL;
    Vector* expected = NULL;

    input = new_vector(&t_immc);
    vector_push(input, new_int_data_immc(IMMC_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 8)));
    expected = new_vector(&t_x64);
    vector_push(expected, new_int_data_x64(X64_DATA_BYTE, new_signed_iliteral(INTEGER_INT, 8)));
    run_data_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_int_data_immc(IMMC_DATA_WORD, new_signed_iliteral(INTEGER_INT, 6)));
    expected = new_vector(&t_x64);
    vector_push(expected, new_int_data_x64(X64_DATA_WORD, new_signed_iliteral(INTEGER_INT, 6)));
    run_data_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_int_data_immc(IMMC_DATA_LONG, new_signed_iliteral(INTEGER_INT, 19)));
    expected = new_vector(&t_x64);
    vector_push(expected, new_int_data_x64(X64_DATA_LONG, new_signed_iliteral(INTEGER_INT, 19)));
    run_data_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_int_data_immc(IMMC_DATA_QUAD, new_signed_iliteral(INTEGER_INT, 551)));
    expected = new_vector(&t_x64);
    vector_push(expected, new_int_data_x64(X64_DATA_QUAD, new_signed_iliteral(INTEGER_INT, 551)));
    run_data_x64gen_test(input, expected);
    delete_vector(expected);

    input = new_vector(&t_immc);
    vector_push(input, new_int_data_immc(IMMC_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 10)));
    expected = new_vector(&t_x64);
    vector_push(expected, new_int_data_x64(X64_DATA_ZERO, new_signed_iliteral(INTEGER_INT, 10)));
    run_data_x64gen_test(input, expected);
    delete_vector(expected);
}

void test_x64gen_data_str() {
    char sliteral_const[15] = "string\0literal";
    int sliteral_size = 15;

    char* immc_sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(immc_sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* immc_sliteral = new_sliteral(immc_sliteral_value, sliteral_size);

    char* x64_sliteral_value = malloc(sliteral_size * sizeof(char));
    memcpy(x64_sliteral_value, sliteral_const, sliteral_size * sizeof(char));

    StringLiteral* x64_sliteral = new_sliteral(x64_sliteral_value, sliteral_size);

    Vector* input = new_vector(&t_immc);
    vector_push(input, new_str_data_immc(IMMC_DATA_STR, immc_sliteral));

    Vector* expected = new_vector(&t_x64);
    vector_push(expected, new_str_data_x64(X64_DATA_STR, x64_sliteral));

    run_data_x64gen_test(input, expected);

    delete_vector(expected);
}

void run_data_x64gen_test(Vector* input, Vector* expected) {
    Vector* liveseqs = new_vector(&t_liveseq);
    vector_fill(liveseqs, 8, new_liveseq());
    X64gen* x64gen = new_x64gen(input, liveseqs);

    Vector* actual = gen_data_x64code(x64gen);

    testlib_assert_x64codes_equal(actual, expected);

    if (actual != NULL) delete_vector(actual);
    delete_x64gen(x64gen);
}
