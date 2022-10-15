#include "./test_suffix.h"
#include "../../src/x64/suffix.h"

void test_x64suffix_get(void);
void test_x64suffix_tonbytes(void);
void test_x64suffix_tochar(void);
void test_x64suffix_greater(void);

CU_Suite* add_test_suite_x64suffix(void) {
    CU_Suite* suite = CU_add_suite("test_suite_x64suffix", NULL, NULL);
    CU_ADD_TEST(suite, test_x64suffix_get);
    CU_ADD_TEST(suite, test_x64suffix_tonbytes);
    CU_ADD_TEST(suite, test_x64suffix_tochar);
    CU_ADD_TEST(suite, test_x64suffix_greater);
    return suite;
}

void test_x64suffix_get(void) {
    CU_ASSERT_EQUAL(x64suffix_get(0), X64_SUFFIX_NONE);
    CU_ASSERT_EQUAL(x64suffix_get(1), X64_SUFFIX_BYTE);
    CU_ASSERT_EQUAL(x64suffix_get(2), X64_SUFFIX_WORD);
    CU_ASSERT_EQUAL(x64suffix_get(4), X64_SUFFIX_LONG);
    CU_ASSERT_EQUAL(x64suffix_get(8), X64_SUFFIX_QUAD);
    CU_ASSERT_EQUAL(x64suffix_get(16), X64_SUFFIX_NONE);
}

void test_x64suffix_tonbytes(void) {
    CU_ASSERT_EQUAL(x64suffix_tonbytes(X64_SUFFIX_NONE), 0);
    CU_ASSERT_EQUAL(x64suffix_tonbytes(X64_SUFFIX_BYTE), 1);
    CU_ASSERT_EQUAL(x64suffix_tonbytes(X64_SUFFIX_WORD), 2);
    CU_ASSERT_EQUAL(x64suffix_tonbytes(X64_SUFFIX_LONG), 4);
    CU_ASSERT_EQUAL(x64suffix_tonbytes(X64_SUFFIX_QUAD), 8);
}

void test_x64suffix_tochar(void) {
    CU_ASSERT_EQUAL(x64suffix_tochar(X64_SUFFIX_NONE), '\0');
    CU_ASSERT_EQUAL(x64suffix_tochar(X64_SUFFIX_BYTE), 'b');
    CU_ASSERT_EQUAL(x64suffix_tochar(X64_SUFFIX_WORD), 'w');
    CU_ASSERT_EQUAL(x64suffix_tochar(X64_SUFFIX_LONG), 'l');
    CU_ASSERT_EQUAL(x64suffix_tochar(X64_SUFFIX_QUAD), 'q');
}

void test_x64suffix_greater(void) {
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_NONE, X64_SUFFIX_NONE), X64_SUFFIX_NONE);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_NONE, X64_SUFFIX_BYTE), X64_SUFFIX_BYTE);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_NONE, X64_SUFFIX_WORD), X64_SUFFIX_WORD);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_NONE, X64_SUFFIX_LONG), X64_SUFFIX_LONG);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_NONE, X64_SUFFIX_QUAD), X64_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_BYTE, X64_SUFFIX_BYTE), X64_SUFFIX_BYTE);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_BYTE, X64_SUFFIX_WORD), X64_SUFFIX_WORD);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_BYTE, X64_SUFFIX_LONG), X64_SUFFIX_LONG);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_BYTE, X64_SUFFIX_QUAD), X64_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_WORD, X64_SUFFIX_WORD), X64_SUFFIX_WORD);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_WORD, X64_SUFFIX_LONG), X64_SUFFIX_LONG);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_WORD, X64_SUFFIX_QUAD), X64_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_LONG, X64_SUFFIX_LONG), X64_SUFFIX_LONG);
    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_LONG, X64_SUFFIX_QUAD), X64_SUFFIX_QUAD);

    CU_ASSERT_EQUAL(x64suffix_greater(X64_SUFFIX_QUAD, X64_SUFFIX_QUAD), X64_SUFFIX_QUAD);
}
