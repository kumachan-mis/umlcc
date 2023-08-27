void put_int(int x);
void put_blank_line(void);

void equality(void) {
    put_int(5 == 5);
    put_int(5 == 4);
    put_blank_line();

    put_int(5 != 5);
    put_int(5 != 4);
    put_blank_line();
}

void relational(void) {
    put_int(5 < 6);
    put_int(5 < 5);
    put_int(5 < 4);
    put_blank_line();

    put_int(5 > 6);
    put_int(5 > 5);
    put_int(5 > 4);
    put_blank_line();

    put_int(5 <= 6);
    put_int(5 <= 5);
    put_int(5 <= 4);
    put_blank_line();

    put_int(5 >= 6);
    put_int(5 >= 5);
    put_int(5 >= 4);
    put_blank_line();
}

int main(void) {
    equality();
    relational();
    return 0;
}
