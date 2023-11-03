void put_int(int x);
void put_blank_line(void);

int main(void) {
    put_int(0x0 & 0x1);
    put_int(0x0 & 0x2);
    put_int(0x1 & 0x1);
    put_int(0x1 & 0x2);
    put_blank_line();

    put_int(0x0 | 0x1);
    put_int(0x0 | 0x2);
    put_int(0x1 | 0x1);
    put_int(0x1 | 0x2);
    put_blank_line();

    put_int(0x0 ^ 0x1);
    put_int(0x0 ^ 0x2);
    put_int(0x1 ^ 0x1);
    put_int(0x1 ^ 0x2);
    put_blank_line();

    put_int(~0x0);
    put_int(~0x1);
    put_int(~0x2);
    put_int(~0x3);
    put_blank_line();

    put_int(1 << 5);
    put_int(3 << 4);
    put_blank_line();

    put_int(64 >> 5);
    put_int(128 >> 4);
    put_blank_line();

    return 0;
}
