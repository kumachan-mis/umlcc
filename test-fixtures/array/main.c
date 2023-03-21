void put_int(int x);
void put_blank_line(void);

int sum(int a[5]) {
    return a[0] + a[1] + a[2] + a[3] + a[4];
}

int main(void) {
    int a[5];
    a[0] = 3;
    a[1] = a[0] + 2;
    a[2] = a[0] + a[1];
    *(a + 3) = *a - 2;
    *(a + 4) = *(a + 3) - *a;

    put_int(*a);
    put_int(*(a + 1));
    put_int(*(a + 2));
    put_int(a[3]);
    put_int(a[4]);
    put_int(&a[4] - &a[1]);
    put_blank_line();

    int b[3];
    0 [b] = 10;
    1 [b] = 11;
    2 [b] = 12;
    put_int(0 [b]);
    put_int(1 [b]);
    put_int(2 [b]);
    put_blank_line();

    int c[3][3];
    c[1][2] = 3;
    put_int(c[1][2]);
    put_blank_line();

    int d[5] = {1, 2, 3, 5, 8};
    put_int(sum(d));

    return 0;
}
