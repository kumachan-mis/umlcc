void put_int(int x);
void put_blank_line(void);

void simple_assignment(void) {
    int first, second, third;

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
    put_blank_line();
}

void compound_assignment(void) {
    int x;

    x = 3;
    put_int(x += 2);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x -= 1);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x *= 2);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x /= 2);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x %= 3);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x &= 1);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x |= 1);
    put_int(x);
    put_blank_line();

    x = 3;
    put_int(x ^= 1);
    put_int(x);
    put_blank_line();
}

int main(void) {
    simple_assignment();
    compound_assignment();

    return 0;
}
