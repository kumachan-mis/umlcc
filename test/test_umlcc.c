int add2(int a, int b) {
    a + b;
}

int add8(int a, int b, int c, int d, int e, int f, int g, int h) {
    a + b + c + d + e + f + g + h;
}

int main() {
    int a, b, c;
    a = 1 + 2 * (5 - 3);
    b = add2(1, 1);
    c = add8(1, 2, 3, 4, 5, 6, 7, 8);
    c - (a + 2 / b);
}
