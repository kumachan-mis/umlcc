void put_int(int x);

int grandchild(int x, int y) {
    return x * x + y * y;
}

int child(int a, int b) {
    int c, d;
    c = a + b;
    d = a - b;
    return grandchild(c, d) - grandchild(a, b);
}

int main() {
    put_int(child(1, 3));
    put_int(child(4, 3));
    return 0;
}
