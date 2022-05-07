#include <stdio.h>

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

int put_blank_line() {
    printf("\n");
    return 0;
}
