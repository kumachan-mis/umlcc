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

    int z, w;
    int *r = &x, *s = &w;

    *r = *s = 7;

    put_int(*&x);
    put_int(*&y);
    put_int(*p == x);
    put_int(*q == y);
    put_int(p - &x == 0);
    put_int(q - &y == 0);
    put_int((p + n) - n == p);
    put_int((q + n) - n == q);
    put_int(*r);
    put_int(*s);

    return 0;
}
