void put_int(int x);
void* malloc(int n);
void free(void*);

typedef struct LinkedList LinkedList;

struct LinkedList {
    int value;
    LinkedList* next;
};

LinkedList* new_linkedlist(int value) {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->value = value;
    list->next = (void*)0;
    return list;
}

int linkedlist_at(LinkedList* list, int n) {
    int ret = 0 - 1;
    ret == 0 - 1 && list != (LinkedList*)0 && n == 0 && (ret = list->value);
    ret == 0 - 1 && list != (LinkedList*)0 && (ret = linkedlist_at(list->next, n - 1));
    return ret;
}

int linkedlist_length(LinkedList* list) {
    int ret = 0;
    list && (ret = linkedlist_length(list->next) + 1);
    return ret;
}

int delete_linkedlist(LinkedList* list) {
    list->next&& delete_linkedlist(list->next);
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

    free(list);

    return 0;
}
