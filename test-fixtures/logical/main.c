void put_int(int x);
void put_blank_line();

int main() {
    put_int(2 && 1);
    put_int(0 && 1);
    put_int(2 && 0);
    put_int(0 && 0);
    put_blank_line();

    put_int(2 || 1);
    put_int(0 || 1);
    put_int(2 || 0);
    put_int(0 || 0);
    put_blank_line();

    put_int(!2);
    put_int(!!1);
    put_int(!0);
    put_int(!!0);
    put_blank_line();

    put_int(7) && put_int(4);
    put_blank_line();

    put_int(0) && put_int(4);
    put_blank_line();

    put_int(7) && put_int(0);
    put_blank_line();

    put_int(0) && put_int(0);
    put_blank_line();

    put_int(7) || put_int(4);
    put_blank_line();

    put_int(0) || put_int(4);
    put_blank_line();

    put_int(7) || put_int(0);
    put_blank_line();

    put_int(0) || put_int(0);

    return 0;
}
