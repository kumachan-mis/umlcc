typedef int test_int;
typedef int* test_intptr;

test_int put_int(test_int x);

int main() {
    test_int a, b;
    test_intptr p, q;
    a = 8;
    b = 13;
    p = &a;
    q = &q;

    put_int(a);
    put_int(b);
    put_int(*p);
    put_int(*q);
    put_int(p == &a);
    put_int(q == &b);

    return 0;
}
