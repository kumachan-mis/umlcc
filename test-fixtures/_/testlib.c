#include <stdio.h>
#include <stdlib.h>

char put_char(char c) {
    printf("%c\n", c);
    return c;
}

int put_int(int x) {
    printf("%d\n", x);
    return x;
}

int put_7ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7) {
    printf("%d\n", x1);
    printf("%d\n", x2);
    printf("%d\n", x3);
    printf("%d\n", x4);
    printf("%d\n", x5);
    printf("%d\n", x6);
    printf("%d\n", x7);
    return 0;
}

int put_8ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) {
    printf("%d\n", x1);
    printf("%d\n", x2);
    printf("%d\n", x3);
    printf("%d\n", x4);
    printf("%d\n", x5);
    printf("%d\n", x6);
    printf("%d\n", x7);
    printf("%d\n", x8);
    return 0;
}

int put_str(char* x) {
    printf("%s\n", x);
    return 0;
}

int put_blank_line(void) {
    printf("\n");
    return 0;
}

typedef struct Node {
    struct Node* next;
    int value;
} Node;

Node* new_node(int value) {
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}

int delete_node(Node* node) {
    if (node->next != NULL) delete_node(node->next);
    free(node);
    return 0;
}
