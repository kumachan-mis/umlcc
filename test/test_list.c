#include "../src/list/list.h"

#include <stdlib.h>
#include <assert.h>


void test_list_push() {
    List* list = new_list();
    int* item = NULL;
    assert(list_length(list) == 0);

    item = malloc(sizeof(int));
    *item = 2;
    list_push(list, item);
    assert(list_length(list) == 1);

    item = malloc(sizeof(int));
    *item = 3;
    list_push(list, item);
    assert(list_length(list) == 2);

    item = malloc(sizeof(int));
    *item = 5;
    list_push(list, item);
    assert(list_length(list) == 3);

    item = malloc(sizeof(int));
    *item = 6;
    list_push(list, item);
    assert(list_length(list) == 4);

    item = list_at(list, 0);
    assert(*item == 2);

    item = list_at(list, 1);
    assert(*item == 3);

    item = list_at(list, 2);
    assert(*item == 5);

    item = list_at(list, 3);
    assert(*item == 6);

    assert(list_length(list) == 4);
    delete_list(list);
}

void test_list_pop() {
    List* list = new_list();
    int* item = NULL;

    item = malloc(sizeof(int));
    *item = -3;
    list_push(list, item);

    item = malloc(sizeof(int));
    *item = -2;
    list_push(list, item);

    item = malloc(sizeof(int));
    *item = 0;
    list_push(list, item);

    assert(list_length(list) == 3);

    item = list_pop(list);
    assert(*item == 0);
    assert(list_length(list) == 2);
    free(item);

    item = list_pop(list);
    assert(*item == -2);
    assert(list_length(list) == 1);
    free(item);

    item = list_pop(list);
    assert(*item == -3);
    assert(list_length(list) == 0);
    free(item);

    delete_list(list);
}
