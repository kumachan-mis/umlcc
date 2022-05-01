#include "./test_map.h"
#include "./test_vector.h"

#include <CUnit/Basic.h>

int main(void) {
    CU_initialize_registry();

    add_test_suite_vector();
    add_test_suite_map();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    int exit_code = CU_get_number_of_failures();
    CU_cleanup_registry();
    return exit_code;
}
