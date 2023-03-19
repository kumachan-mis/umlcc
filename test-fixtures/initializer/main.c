void put_int(int x);
void put_blank_line(void);

int main(void) {
    int x = 8;
    int y = {x};

    put_int(x);
    put_int(y);
    put_blank_line();

    int a[2] = {1};
    put_int(a[0]);
    put_int(a[1]);
    put_blank_line();

    int b[2][3] = {{2}, {3, 4, 5}};
    put_int(b[0][0]);
    put_int(b[0][1]);
    put_int(b[0][2]);
    put_int(b[1][0]);
    put_int(b[1][1]);
    put_int(b[1][2]);
    put_blank_line();

    int c[3][2] = {x + 1, x * 2, x + 4};
    put_int(c[0][0]);
    put_int(c[0][1]);
    put_int(c[1][0]);
    put_int(c[1][1]);
    put_int(c[2][0]);
    put_int(c[2][0]);
    put_blank_line();

    int d[3][3] = {{10}, 11, 12, 13, {14}};
    put_int(d[0][0]);
    put_int(d[0][1]);
    put_int(d[0][2]);
    put_int(d[1][0]);
    put_int(d[1][1]);
    put_int(d[1][2]);
    put_int(d[2][0]);
    put_int(d[2][1]);
    put_int(d[2][2]);
    put_blank_line();

    int e[2][3] = {20, {21}, {22}};
    put_int(e[0][0]);
    put_int(e[0][1]);
    put_int(e[0][2]);
    put_int(e[1][0]);
    put_int(e[1][1]);
    put_int(e[1][2]);

    return 0;
}
