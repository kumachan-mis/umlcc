#include "./test_liveseq.h"
#include "../../src/liveseq/liveness.h"
#include "../../src/liveseq/liveseq.h"

void test_liveseq_copy(void);
void test_liveseq_empty(void);
void test_liveseq_unused_liveness(void);
void test_liveseq_starts_with_zero(void);
void test_liveseq_starts_with_nonzero(void);
void test_liveseq_redef_without_interval(void);

CU_Suite* add_test_suite_liveseq(void) {
    CU_Suite* suite = CU_add_suite("test_suite_liveseq", NULL, NULL);
    CU_ADD_TEST(suite, test_liveseq_copy);
    CU_ADD_TEST(suite, test_liveseq_empty);
    CU_ADD_TEST(suite, test_liveseq_unused_liveness);
    CU_ADD_TEST(suite, test_liveseq_starts_with_zero);
    CU_ADD_TEST(suite, test_liveseq_starts_with_nonzero);
    CU_ADD_TEST(suite, test_liveseq_redef_without_interval);
    return suite;
}

void test_liveseq_copy(void) {
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

void test_liveseq_empty(void) {
    Liveseq* liveseq = new_liveseq();

    CU_ASSERT_EQUAL(vector_size(liveseq->livenesses), 0);
    CU_ASSERT_EQUAL(liveseq->index, 0);
    CU_ASSERT_FALSE(liveseq_isalive(liveseq));

    liveseq_goto_next(liveseq);

    CU_ASSERT_EQUAL(liveseq->index, 1);
    CU_ASSERT_FALSE(liveseq_isalive(liveseq));

    delete_liveseq(liveseq);
}

void test_liveseq_unused_liveness(void) {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(0);
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(2);
    vector_push(liveseq->livenesses, liveness);

    int expected_isalives[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_isalive(liveseq), expected_isalives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}

void test_liveseq_starts_with_zero(void) {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(0);
    liveness->last_use_index = 2;
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(4);
    liveness->last_use_index = 5;
    vector_push(liveseq->livenesses, liveness);

    int expected_isalives[8] = {1, 1, 0, 0, 1, 0, 0, 0};
    for (int i = 0; i < 8; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_isalive(liveseq), expected_isalives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}

void test_liveseq_starts_with_nonzero(void) {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(4);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    int expected_isalives[8] = {0, 0, 1, 0, 1, 1, 0, 0};
    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_isalive(liveseq), expected_isalives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}

void test_liveseq_redef_without_interval(void) {
    Liveseq* liveseq = new_liveseq();
    Liveness* liveness = NULL;

    liveness = new_liveness(2);
    liveness->last_use_index = 3;
    vector_push(liveseq->livenesses, liveness);

    liveness = new_liveness(3);
    liveness->last_use_index = 6;
    vector_push(liveseq->livenesses, liveness);

    int expected_isalives[8] = {0, 0, 1, 1, 1, 1, 0, 0};
    for (int i = 0; i < 7; i++) {
        CU_ASSERT_EQUAL(liveseq->index, i);
        CU_ASSERT_EQUAL(liveseq_isalive(liveseq), expected_isalives[i]);
        liveseq_goto_next(liveseq);
    }

    delete_liveseq(liveseq);
}
