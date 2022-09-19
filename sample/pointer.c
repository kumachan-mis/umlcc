int* addr(void) {
    int x = 3;
    int* p = &x;
    return p;
}

int indir(void) {
    int a = 3;
    int* p = &a;
    int b = *p;
    return b;
}

int* add(void) {
    int x = 3;
    int* a = &x;
    int b = 1;
    int* c = a + b;
    return c;
}

int* sub(void) {
    int x = 3;
    int* a = &x;
    int b = 1;
    int* c = a - b;
    return c;
}

int diff(void) {
    int x = 3;
    int y = 6;
    int* a = &x;
    int* b = &y;
    int c = a - b;
    return c;
}
