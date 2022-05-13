#include "./test_set.h"
#include "../src/common/common.h"
#include "../src/set/set.h"

#include <CUnit/Basic.h>

void test_set_add();
void test_set_remove();
void test_set_intersection();
void test_set_union();
void test_set_difference();

CU_Suite* add_test_suite_set() {
    CU_Suite* suite = CU_add_suite("test_suite_set", NULL, NULL);
    CU_add_test(suite, "test_set_add", test_set_add);
    CU_add_test(suite, "test_set_remove", test_set_remove);
    CU_add_test(suite, "test_set_intersection", test_set_intersection);
    CU_add_test(suite, "test_set_union", test_set_union);
    CU_add_test(suite, "test_set_intersection", test_set_difference);
    return suite;
}

void test_set_add() {

}

void test_set_remove() {

}

void test_set_intersection() {

}

void test_set_union() {

}

void test_set_difference() {

}
