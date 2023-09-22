void put_int(int x);
void* malloc(int n);
void free(void*);

typedef struct LinkedList LinkedList;

typedef struct LinkedList {
    int value;
    LinkedList* next;
};

LinkedList* new_linkedlist(int value) {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->value = value;
    list->next = (LinkedList*)0;
    return list;
}

int linkedlist_at(LinkedList* list, int n) {
    if (!list) {
        return -1;
    }
    if (n == 0) {
        return list->value;
    }
    return linkedlist_at(list->next, n - 1);
}

int linkedlist_length(LinkedList* list) {
    if (!list) {
        return 0;
    }
    return linkedlist_length(list->next) + 1;
}

int delete_linkedlist(LinkedList* list) {
    if (list->next) {
        delete_linkedlist(list->next);
    }
    free(list);
    return 0;
}

int main(void) {
    LinkedList* list = new_linkedlist(2);
    LinkedList* node = list;

    node->next = new_linkedlist(3);
    node = node->next;

    node->next = new_linkedlist(5);
    node = node->next;

    node->next = new_linkedlist(8);
    node = node->next;

    put_int(linkedlist_length(list));
    put_int(linkedlist_at(list, 0));
    put_int(linkedlist_at(list, 1));
    put_int(linkedlist_at(list, 2));
    put_int(linkedlist_at(list, 3));
    put_int(linkedlist_at(list, 4));
    put_int(linkedlist_at(list, 5));

    delete_linkedlist(list);

    return 0;
}
