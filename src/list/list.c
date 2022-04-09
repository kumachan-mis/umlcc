#include "./list.h"

#include <stdlib.h>

struct _ListItem {
    void* _data;
    struct _ListItem* _next;
};

List* new_list() {
    List* list = malloc(sizeof(List));
    list->_head = NULL;
    return list;
}

void delete_list(List* list) {
    ListItem* item = list->_head;
    while (item != NULL) {
        ListItem* next = item->_next;
        free(item->_data);
        free(item);
        item = next;
    }
    free(list);
}

void list_push(List* list, void* data) {
    ListItem* item = malloc(sizeof(ListItem));
    item->_data = data;
    item->_next = NULL;

    if (list->_head == NULL) {
        list->_head = item;
        return;
    }
    
    ListItem* tail = list->_head;
    while (tail->_next != NULL) {
        tail = tail->_next;
    }
    tail->_next = item;
}

void* list_pop(List* list) {
    if (list->_head == NULL) return NULL;

    if (list->_head->_next == NULL) {
        void* data = list->_head->_data;
        free(list->_head);
        list->_head = NULL;
        return data;
    }

    ListItem* before_tail = list->_head;
    while (before_tail->_next->_next != NULL) {
        before_tail = before_tail->_next;
    }
    void* data = before_tail->_next->_data;
    free(before_tail->_next);
    before_tail->_next = NULL;
    return data;
}

void* list_at(List* list, int index) {
    ListItem* item = list->_head;
    while (index > 0 && item != NULL) {
        item = item->_next;
        index--;
    }
    return item->_data;
}

int list_length(List* list) {
    int length = 0;
    ListItem* item = list->_head;
    while (item != NULL) {
        item = item->_next;
        length++;
    }
    return length;
}
