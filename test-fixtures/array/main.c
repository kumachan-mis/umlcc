int put_int(int x);

int main() {
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

    return 0;
}
