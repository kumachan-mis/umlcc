void put_char(char c);
void put_int(int x);
void put_str(char* x);

int main() {
    char x = 30, y = 40, z = 10;
    char w = (x * y) / z;
    put_int(w);

    char c1, c2;
    c1 = 'a';
    c2 = 'ab';
    put_char(c1);
    put_char(c2);
    put_int(c1 == c2);
    c1 = 'b';
    c2 = 'ba';
    put_char(c1);
    put_char(c2);
    put_int(c1 == c2);

    char null_char = '\0';
    put_int(null_char);

    char s[4] = {'a', 'b', 'c', '\0'};
    put_str(s);

    return 0;
}
