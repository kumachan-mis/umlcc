void put_int(int x);
void put_unsigned_int(unsigned int x);
void put_long_long(long long x);
void put_unsigned_long_long(unsigned long long x);
void put_blank_line(void);

void int_arithmetic(void) {
    put_int(1 + 2);
    put_int(3 - 8);
    put_int(4 * 2);
    put_int(17 / 4);
    put_blank_line();
}

void unsigned_int_arithmetic(void) {
    put_unsigned_int(1 + 2u);
    put_unsigned_int(8u - 3);
    put_unsigned_int(4u * 2);
    put_unsigned_int(17u / 4u);
    put_blank_line();
}

void long_long_arithmetic(void) {
    put_long_long(1 + 2ll);
    put_long_long(3ll - 8u);
    put_long_long(4 * 2ll);
    put_long_long(17u / 4ll);
    put_blank_line();
}

void unsigned_long_long_arithmetic(void) {
    put_unsigned_long_long(1 + 2ull);
    put_unsigned_long_long(8ull - 3);
    put_unsigned_long_long(4ull * 2);
    put_unsigned_long_long(17u / 4ull);
    put_blank_line();
}

int main(void) {
    int_arithmetic();
    unsigned_int_arithmetic();
    long_long_arithmetic();
    unsigned_long_long_arithmetic();
    return 0;
}
