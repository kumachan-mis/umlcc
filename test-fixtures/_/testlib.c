#include <stdio.h>
#include <stdlib.h>

void put_char(char c) {
    printf("%c\n", c);
}

void put_int(int x) {
    printf("%d\n", x);
}

void put_7ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7) {
    printf("%d\n", x1);
    printf("%d\n", x2);
    printf("%d\n", x3);
    printf("%d\n", x4);
    printf("%d\n", x5);
    printf("%d\n", x6);
    printf("%d\n", x7);
}

void put_8ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) {
    printf("%d\n", x1);
    printf("%d\n", x2);
    printf("%d\n", x3);
    printf("%d\n", x4);
    printf("%d\n", x5);
    printf("%d\n", x6);
    printf("%d\n", x7);
    printf("%d\n", x8);
}

void put_str(char* x) {
    printf("%s\n", x);
}

void put_blank_line(void) {
    printf("\n");
}

typedef struct Node {
    int value;
    struct Node* next;
} Node;

Node* new_node(int value) {
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}

void delete_node(Node* node) {
    if (node->next != NULL) delete_node(node->next);
    free(node);
}
