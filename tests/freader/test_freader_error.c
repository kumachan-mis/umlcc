#include "./test_freader_error.h"
#include "../../src/freader/freader.h"
#include "../testlib/testlib.h"

#include <stdlib.h>

void test_freader_error_empty(void);

CU_Suite* add_test_suite_freader_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_freader_error", NULL, NULL);
    CU_ADD_TEST(suite, test_freader_error_empty);
    return suite;
}

void test_freader_error_empty(void) {
    Error* expected = new_error("fail to read content of file");

    FILE* file_ptr = tmpfile();
    fprintf(file_ptr, "%s", "");
    rewind(file_ptr);

    FileReader* freader = new_freader(file_ptr);

    char* content = NULL;
    Error* actual = NULL;
    freaderret_assign(&content, &actual, freader_read(freader));

    CU_ASSERT_PTR_NULL(content);
    testlib_assert_error_equal(actual, expected);

    if (actual != NULL) {
        delete_error(actual);
    }
    delete_freader(freader);

    delete_error(expected);
}
