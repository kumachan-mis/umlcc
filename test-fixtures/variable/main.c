int put_int(int x);

int main() {
    int first, second, third;
    first = 3;
    second = first - 1;
    third = first * second;

    put_int(first);
    put_int(second);
    put_int(third);
    put_int(third - (first - second));

    return 0;
}
