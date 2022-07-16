#include "./test_liveness.h"
#include "../../src/liveseq/liveness.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_new_liveness();
void test_liveness_copy();

CU_Suite* add_test_suite_liveness() {
    CU_Suite* suite = CU_add_suite("test_suite_liveness", NULL, NULL);
    CU_add_test(suite, "test_new_liveness", test_new_liveness);
    CU_add_test(suite, "test_liveness_copy", test_liveness_copy);
    return suite;
}

void test_new_liveness() {
    Liveness* liveness = new_liveness(3);

    CU_ASSERT_EQUAL(liveness->first_def_index, 3);
    CU_ASSERT_EQUAL(liveness->last_use_index, 3);

    liveness->last_use_index = 8;
    CU_ASSERT_EQUAL(liveness->first_def_index, 3);
    CU_ASSERT_EQUAL(liveness->last_use_index, 8);

    delete_liveness(liveness);
}

void test_liveness_copy() {
    Liveness* liveness = new_liveness(3);
    Liveness* copied_liveness = liveness_copy(liveness);
    delete_liveness(liveness);

    CU_ASSERT_EQUAL(copied_liveness->first_def_index, 3);
    CU_ASSERT_EQUAL(copied_liveness->last_use_index, 3);

    copied_liveness->last_use_index = 8;
    CU_ASSERT_EQUAL(copied_liveness->first_def_index, 3);
    CU_ASSERT_EQUAL(copied_liveness->last_use_index, 8);

    delete_liveness(copied_liveness);
}
