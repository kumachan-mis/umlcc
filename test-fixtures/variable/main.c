int put_int(int x);

int main() {
    int a, b, c, d;
    a = 3;
    b = a - 1;
    c = a * b;

    put_int(a);
    put_int(b);
    put_int(c);
    put_int(c - (a - b));
    0;
}
