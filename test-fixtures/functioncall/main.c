int put_int(int x);
int put_7ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7);
int put_8ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8);
int put_blank_line();

int add_2ints(int x1, int x2) {
    return x1 + x2;
}

int addsub_8ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) {
    return x1 - x2 + x3 - x4 + x5 - x6 + x7 - x8;
}

int main() {
    put_7ints(1, 3, 5, 7, 9, 11, 13);
    put_blank_line();

    put_8ints(2, 4, 8, 16, 32, 64, 128, 256);
    put_blank_line();

    put_int(add_2ints(6, 7));
    put_int(addsub_8ints(1, 2, 3, 4, 5, 6, 7, 8));

    return 0;
}
