#include "./ast/test_ast.h"
#include "./ctoken/test_ctoken.h"
#include "./dtype/test_dtype.h"
#include "./literal/test_sliteral.h"
#include "./liveseq/test_liveness.h"
#include "./liveseq/test_liveseq.h"
#include "./map/test_map.h"
#include "./set/test_set.h"
#include "./srt/test_srt.h"
#include "./symtab/test_symtab.h"
#include "./vector/test_vector.h"

#include <CUnit/Basic.h>

int main(void) {
    CU_initialize_registry();

    // container tests
    add_test_suite_vector();
    add_test_suite_map();
    add_test_suite_set();

    // intermediate expression tests
    add_test_suite_ctoken();
    add_test_suite_ast();
    add_test_suite_srt();
    add_test_suite_sliteral();
    add_test_suite_dtype();
    add_test_suite_symboltable();
    add_test_suite_liveness();
    add_test_suite_liveseq();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    int exit_code = CU_get_number_of_failures();
    CU_cleanup_registry();
    return exit_code;
}
