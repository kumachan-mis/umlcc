int put_int(int x);

int main() {
    char x = 30, y = 40, z = 10;
    char w = (x * y) / z;
    put_int(w);

    char c1 = 'a', c2 = 'ab';
    put_int(c1 == c2);

    char null_char = '\0';
    put_int(null_char);

    return 0;
}
