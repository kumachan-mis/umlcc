#include "./test_error.h"
#include "../../src/error/error.h"

#include <stdlib.h>

void test_new_error(void);
void test_error_copy(void);

CU_Suite* add_test_suite_error(void) {
    CU_Suite* suite = CU_add_suite("test_suite_error", NULL, NULL);
    CU_ADD_TEST(suite, test_new_error);
    CU_ADD_TEST(suite, test_error_copy);
    return suite;
}

void test_new_error(void) {
    Error* err = new_error("L%d:%d: %s raised an error", 10, 4, "test_function");

    CU_ASSERT_STRING_EQUAL(err->message, "L10:4: test_function raised an error\n");

    delete_error(err);
}

void test_error_copy(void) {
    Error* err = new_error("error is copied");
    Error* copied_err = erorr_copy(err);
    delete_error(err);

    CU_ASSERT_STRING_EQUAL(copied_err->message, "error is copied\n");

    delete_error(copied_err);
}
