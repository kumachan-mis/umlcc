typedef int test_int, *test_intptr;

void put_int(test_int x);
void put_str(char* s);
void put_blank_line(void);

int main(void) {
    test_int a, b;
    test_intptr p, q;
    a = 8;
    b = 13;
    p = &a;
    q = &b;

    put_int(a);
    put_int(b);
    put_int(*p);
    put_int(*q);
    put_int(p == &a);
    put_int(q == &b);
    put_blank_line();

    char typedef test_char;
    test_char x = 20, y = 40, z = 50, w = 10;
    put_int((x * y + z) / w);
    {
        char typedef* string;
        string s = "ptr string";
        put_str(s);
    }

    {
        char typedef string[14];
        string s = "array string";
        put_str(s);
        put_int(s[13]);
    }

    return 0;
}
