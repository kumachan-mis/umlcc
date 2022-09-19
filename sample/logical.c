int lor(void) {
    int a = 14;
    int b = 3;
    int c = a || b;
    return c;
}

int land(void) {
    int a = 14;
    int b = 3;
    int c = a && b;
    return c;
}

int lnot(void) {
    int a = 2;
    int b = !a;
    return b;
}
