#include "./test_basicblock.h"
#include "../../src/immc/immc.h"
#include "../../src/regalloc/basicblock.h"

void test_new_basicblock();
void test_basicblock_copy();

CU_Suite* add_test_suite_basicblock() {
    CU_Suite* suite = CU_add_suite("test_suite_basicblock", NULL, NULL);
    CU_ADD_TEST(suite, test_new_basicblock);
    CU_ADD_TEST(suite, test_basicblock_copy);
    return suite;
}

void test_new_basicblock() {
    Vector* immcs = new_vector(&t_immc);
    BasicBlock* basicblock = new_basicblock(immcs);

    CU_ASSERT_PTR_EQUAL(basicblock->immcs, immcs);

    int count = 0;
    for (SetIter* iter = set_iter_begin(basicblock->input); !set_iter_end(iter, basicblock->input);
         iter = set_iter_next(iter, basicblock->input)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);

    count = 0;
    for (SetIter* iter = set_iter_begin(basicblock->output); !set_iter_end(iter, basicblock->output);
         iter = set_iter_next(iter, basicblock->output)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);

    count = 0;
    for (SetIter* iter = set_iter_begin(basicblock->succ); !set_iter_end(iter, basicblock->succ);
         iter = set_iter_next(iter, basicblock->succ)) {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);

    delete_basicblock(basicblock);
}

void test_basicblock_copy() {
    Vector* immcs = new_vector(&t_immc);
    BasicBlock* basicblock = new_basicblock(immcs);
    set_add(basicblock->input, new_integer(2));
    set_add(basicblock->output, new_integer(4));
    set_add(basicblock->succ, new_integer(6));

    BasicBlock* copied_basicblock = basicblock_copy(basicblock);
    delete_basicblock(basicblock);

    CU_ASSERT_EQUAL(vector_size(copied_basicblock->immcs), 0);

    int count = 0;
    for (SetIter* iter = set_iter_begin(copied_basicblock->input); !set_iter_end(iter, copied_basicblock->input);
         iter = set_iter_next(iter, copied_basicblock->input)) {
        int* item = set_iter_item(iter, copied_basicblock->input);
        CU_ASSERT(*item == 2);
        count++;
    }
    CU_ASSERT_EQUAL(count, 1);

    count = 0;
    for (SetIter* iter = set_iter_begin(copied_basicblock->output); !set_iter_end(iter, copied_basicblock->output);
         iter = set_iter_next(iter, copied_basicblock->output)) {
        int* item = set_iter_item(iter, copied_basicblock->output);
        CU_ASSERT(*item == 4);
        count++;
    }
    CU_ASSERT_EQUAL(count, 1);

    count = 0;
    for (SetIter* iter = set_iter_begin(copied_basicblock->succ); !set_iter_end(iter, copied_basicblock->succ);
         iter = set_iter_next(iter, copied_basicblock->succ)) {
        count++;
        int* item = set_iter_item(iter, copied_basicblock->succ);
        CU_ASSERT(*item == 6);
    }
    CU_ASSERT_EQUAL(count, 1);

    delete_basicblock(copied_basicblock);
}
