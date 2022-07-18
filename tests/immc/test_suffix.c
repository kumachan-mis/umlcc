#include "./test_suffix.h"
#include "../../src/immc/suffix.h"

void test_immcsuffix_get();
void test_immcsuffix_tosize();
void test_immcsuffix_tochar();
void test_immcsuffix_greater();

CU_Suite* add_test_suite_immcsuffix() {
    CU_Suite* suite = CU_add_suite("test_suite_immcsuffix", NULL, NULL);
    CU_ADD_TEST(suite, test_immcsuffix_get);
    CU_ADD_TEST(suite, test_immcsuffix_tosize);
    CU_ADD_TEST(suite, test_immcsuffix_tochar);
    CU_ADD_TEST(suite, test_immcsuffix_greater);
    return suite;
}

void test_immcsuffix_get() {
    CU_ASSERT_EQUAL(immcsuffix_get(0), IMMC_SUFFIX_NONE);
    CU_ASSERT_EQUAL(immcsuffix_get(1), IMMC_SUFFIX_BYTE);
    CU_ASSERT_EQUAL(immcsuffix_get(2), IMMC_SUFFIX_WORD);
    CU_ASSERT_EQUAL(immcsuffix_get(4), IMMC_SUFFIX_LONG);
    CU_ASSERT_EQUAL(immcsuffix_get(8), IMMC_SUFFIX_QUAD);
    CU_ASSERT_EQUAL(immcsuffix_get(16), IMMC_SUFFIX_NONE);
}

void test_immcsuffix_tosize() {
    CU_ASSERT_EQUAL(immcsuffix_tosize(IMMC_SUFFIX_NONE), 0);
    CU_ASSERT_EQUAL(immcsuffix_tosize(IMMC_SUFFIX_BYTE), 1);
    CU_ASSERT_EQUAL(immcsuffix_tosize(IMMC_SUFFIX_WORD), 2);
    CU_ASSERT_EQUAL(immcsuffix_tosize(IMMC_SUFFIX_LONG), 4);
    CU_ASSERT_EQUAL(immcsuffix_tosize(IMMC_SUFFIX_QUAD), 8);
}

void test_immcsuffix_tochar() {
    CU_ASSERT_EQUAL(immcsuffix_tochar(IMMC_SUFFIX_NONE), '\0');
    CU_ASSERT_EQUAL(immcsuffix_tochar(IMMC_SUFFIX_BYTE), 'b');
    CU_ASSERT_EQUAL(immcsuffix_tochar(IMMC_SUFFIX_WORD), 'w');
    CU_ASSERT_EQUAL(immcsuffix_tochar(IMMC_SUFFIX_LONG), 'l');
    CU_ASSERT_EQUAL(immcsuffix_tochar(IMMC_SUFFIX_QUAD), 'q');
}

void test_immcsuffix_greater() {
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_NONE, IMMC_SUFFIX_NONE), IMMC_SUFFIX_NONE);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_NONE, IMMC_SUFFIX_BYTE), IMMC_SUFFIX_BYTE);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_NONE, IMMC_SUFFIX_WORD), IMMC_SUFFIX_WORD);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_NONE, IMMC_SUFFIX_LONG), IMMC_SUFFIX_LONG);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_NONE, IMMC_SUFFIX_QUAD), IMMC_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_BYTE, IMMC_SUFFIX_BYTE), IMMC_SUFFIX_BYTE);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_BYTE, IMMC_SUFFIX_WORD), IMMC_SUFFIX_WORD);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_BYTE, IMMC_SUFFIX_LONG), IMMC_SUFFIX_LONG);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_BYTE, IMMC_SUFFIX_QUAD), IMMC_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_WORD, IMMC_SUFFIX_WORD), IMMC_SUFFIX_WORD);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_WORD, IMMC_SUFFIX_LONG), IMMC_SUFFIX_LONG);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_WORD, IMMC_SUFFIX_QUAD), IMMC_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_LONG, IMMC_SUFFIX_LONG), IMMC_SUFFIX_LONG);
    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_LONG, IMMC_SUFFIX_QUAD), IMMC_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(immcsuffix_greater(IMMC_SUFFIX_QUAD, IMMC_SUFFIX_QUAD), IMMC_SUFFIX_QUAD);
}
