void put_int(int x);

int main(void) {
    int first, second, third;
    void put_blank_line(void);

    first = 3;
    second = first - 1;
    third = first * second;

    put_int(first);
    put_int(second);
    put_int(third);
    put_int(third - (first - second));
    put_blank_line();

    third = (first = 9) - (second = 1);
    put_int(first);
    put_int(second);
    put_int(third);

    return 0;
}
