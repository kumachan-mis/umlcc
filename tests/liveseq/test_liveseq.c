#include "./test_liveseq.h"
#include "../../src/liveseq/liveness.h"
#include "../../src/liveseq/liveseq.h"

#include <CUnit/Basic.h>
#include <stdlib.h>

void test_liveseq_copy();
void test_liveseq_empty();
void test_liveseq_unused_liveness();
void test_liveseq_starts_with_zero();
void test_liveseq_starts_with_nonzero();
void test_liveseq_redef_without_interval();

CU_Suite* add_test_suite_liveseq() {
    CU_Suite* suite = CU_add_suite("test_suite_liveseq", NULL, NULL);
    CU_add_test(suite, "test_liveseq_copy", test_liveseq_copy);
    CU_add_test(suite, "test_liveseq_empty", test_liveseq_empty);
    CU_add_test(suite, "test_liveseq_unused_liveness", test_liveseq_unused_liveness);
    CU_add_test(suite, "test_liveseq_starts_with_zero", test_liveseq_starts_with_zero);
    CU_add_test(suite, "test_liveseq_starts_with_nonzero", test_liveseq_starts_with_nonzero);
    CU_add_test(suite, "test_liveseq_redef_without_interval", test_liveseq_redef_without_interval);
    return suite;
}

void test_liveseq_copy() {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    Liveseq* copied_liveseq = liveseq_copy(liveseq);

    CU_ASSERT_EQUAL(copied_liveseq->liveness_index, liveseq->liveness_index);
    CU_ASSERT_EQUAL(copied_liveseq->index, liveseq->index);
    CU_ASSERT_EQUAL(vector_size(copied_liveseq->livenesses), 1);

    liveness = vector_at(copied_liveseq->livenesses, 0);
    CU_ASSERT_EQUAL(liveness->first_def_index, 0);
    CU_ASSERT_EQUAL(liveness->last_use_index, 2);

    delete_liveseq(liveseq);
    delete_liveseq(copied_liveseq);
}

void test_liveseq_empty() {
    Liveseq* liveseq = new_liveseq();

    CU_ASSERT_EQUAL(vector_size(liveseq->livenesses), 0);
    CU_ASSERT_EQUAL(liveseq->index, 0);
    CU_ASSERT_FALSE(liveseq_is_alive(liveseq));

    liveseq_goto_next(liveseq);

    CU_ASSERT_EQUAL(liveseq->index, 1);
    CU_ASSERT_FALSE(liveseq_is_alive(liveseq));

    delete_liveseq(liveseq);
}

void test_liveseq_unused_liveness() {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);

    int expected_is_alives[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_is_alive(liveseq), expected_is_alives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}

void test_liveseq_starts_with_zero() {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(4);
    liveness->last_use_index = 5;
    vector_push(liveseq->livenesses, liveness);

    int expected_is_alives[8] = {1, 1, 0, 0, 1, 0, 0, 0};
    for (int i = 0; i < 8; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_is_alive(liveseq), expected_is_alives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}

void test_liveseq_starts_with_nonzero() {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(4);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    int expected_is_alives[8] = {0, 0, 1, 0, 1, 1, 0, 0};
    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_is_alive(liveseq), expected_is_alives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}

void test_liveseq_redef_without_interval() {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(3);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    int expected_is_alives[8] = {0, 0, 1, 1, 1, 1, 0, 0};
    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_is_alive(liveseq), expected_is_alives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}
