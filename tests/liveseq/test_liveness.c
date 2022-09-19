#include "./test_liveness.h"
#include "../../src/liveseq/liveness.h"

void test_new_liveness(void);
void test_liveness_copy(void);

CU_Suite* add_test_suite_liveness(void) {
    CU_Suite* suite = CU_add_suite("test_suite_liveness", NULL, NULL);
    CU_ADD_TEST(suite, test_new_liveness);
    CU_ADD_TEST(suite, test_liveness_copy);
    return suite;
}

void test_new_liveness(void) {
    Liveness* liveness = new_liveness(3);

    CU_ASSERT_EQUAL(liveness->first_def_index, 3);
    CU_ASSERT_EQUAL(liveness->last_use_index, 3);

    liveness->last_use_index = 8;
    CU_ASSERT_EQUAL(liveness->first_def_index, 3);
    CU_ASSERT_EQUAL(liveness->last_use_index, 8);

    delete_liveness(liveness);
}

void test_liveness_copy(void) {
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
