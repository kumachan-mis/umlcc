#include "./test_freader.h"
#include "../../src/freader/freader.h"

#include <stdlib.h>

void test_freader(void);

CU_Suite* add_test_suite_freader(void) {
    CU_Suite* suite = CU_add_suite("test_suite_freader", NULL, NULL);
    CU_ADD_TEST(suite, test_freader);
    return suite;
}

void test_freader(void) {
    char* expected = new_string("void test_freader(void);");

    FILE* file_ptr = tmpfile();
    fprintf(file_ptr, "%s", expected);
    rewind(file_ptr);

    FileReader* freader = new_freader(file_ptr);

    char* actual = NULL;
    Error* err = NULL;
    freaderret_assign(&actual, &err, freader_read(freader));

    CU_ASSERT_STRING_EQUAL(actual, expected);
    CU_ASSERT_PTR_NULL(err);

    if (actual != NULL) {
        free(actual);
    }
    delete_freader(freader);

    free(expected);
}
