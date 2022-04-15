#include "../src/vector/vector.h"

#include <stdlib.h>
#include <assert.h>


void test_vector_push();
void test_vector_pop();

int main(void) {
    test_vector_push();
    test_vector_pop();
}

void test_vector_push() {
    Vector* vector = new_vector();
    int* item = NULL;
    assert(vector_size(vector) == 0);

    item = malloc(sizeof(int));
    *item = 2;
    vector_push(vector, item);
    assert(vector_size(vector) == 1);

    item = malloc(sizeof(int));
    *item = 3;
    vector_push(vector, item);
    assert(vector_size(vector) == 2);

    item = malloc(sizeof(int));
    *item = 5;
    vector_push(vector, item);
    assert(vector_size(vector) == 3);

    item = malloc(sizeof(int));
    *item = 6;
    vector_push(vector, item);
    assert(vector_size(vector) == 4);

    item = vector_at(vector, 0);
    assert(*item == 2);

    item = vector_at(vector, 1);
    assert(*item == 3);

    item = vector_at(vector, 2);
    assert(*item == 5);

    item = vector_at(vector, 3);
    assert(*item == 6);

    assert(vector_size(vector) == 4);
    delete_vector(vector, free);
}

void test_vector_pop() {
    Vector* vector = new_vector();
    int* item = NULL;

    item = malloc(sizeof(int));
    *item = -3;
    vector_push(vector, item);

    item = malloc(sizeof(int));
    *item = -2;
    vector_push(vector, item);

    item = malloc(sizeof(int));
    *item = 0;
    vector_push(vector, item);

    assert(vector_size(vector) == 3);

    item = vector_pop(vector);
    assert(*item == 0);
    assert(vector_size(vector) == 2);
    free(item);

    item = vector_pop(vector);
    assert(*item == -2);
    assert(vector_size(vector) == 1);
    free(item);

    item = vector_pop(vector);
    assert(*item == -3);
    assert(vector_size(vector) == 0);
    free(item);

    delete_vector(vector, free);
}
