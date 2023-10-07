void put_int(int x);
void put_unsigned_int(unsigned int x);
void put_blank_line(void);

unsigned string_length(char* s) {
    unsigned len = 0;
    while (*s++ != '\0') {
        ++len;
    }
    return len;
}

int main(void) {
    int x;

    x = 1;
    put_int(x++);
    x = 1;
    put_int(++x);
    x = 1;
    put_int(x--);
    x = 1;
    put_int(--x);
    put_blank_line();

    char* s = "Hello, world!";
    put_unsigned_int(string_length(s));

    return 0;
}
