void put_int(int x);

int bin_operator(int operator(int x, int y), int x, int y) {
    return operator(x, y);
}

int add(int x, int y) {
    return x + y;
}

int sub(int x, int y) {
    return x - y;
}

int mul(int x, int y) {
    return x * y;
}

int div(int x, int y) {
    return x / y;
}

int main() {
    put_int(bin_operator(add, 3, 4));
    put_int(bin_operator(sub, 1, 2));
    put_int(bin_operator(mul, 3, 5));
    put_int(bin_operator(div, 6, 5));
    return 0;
}
