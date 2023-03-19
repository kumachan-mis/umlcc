enum Size {
    SMALL = 10,
    MEDIUM = 20,
    LARGE = 30,
    EXTRA_LARGE,
};

void put_int(int x);
void put_blank_line(void);

int main() {
    typedef enum Number { ZERO, ONE } Number;

    put_int(ZERO);
    put_int(ONE);
    put_int(SMALL);
    put_int(MEDIUM);
    put_int(LARGE);
    put_int(EXTRA_LARGE);
    put_blank_line();

    Number p = 2, q = ZERO;
    put_int(p);
    put_int(q);
    put_blank_line();

    enum Size xs = 5, s = SMALL, m = MEDIUM, l = LARGE, xl = EXTRA_LARGE;
    put_int(xs);
    put_int(s);
    put_int(m);
    put_int(l);
    put_int(xl);
}
