int put_int(int x);

int main() {
    int x, y;
    x = 5;
    y = 8;

    int *p, *q;
    p = &x;
    q = &y;

    int n;
    n = 2;

    int a[5];

    put_int(*&x);
    put_int(*&y);
    put_int(*p == x);
    put_int(*q == y);
    put_int(p - &x == 0);
    put_int(q - &y == 0);
    put_int((p + n) - n == p);
    put_int((q + n) - n == q);
    put_int(&a[4] - &a[1]);

    return 0;
}
