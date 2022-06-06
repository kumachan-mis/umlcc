int put_int(int x);
int put_blank_line();

int main() {
    int x = 8;
    int a[2] = {1};
    int b[2][3] = {{2}, {3, 4, 5}};

    put_int(x);
    put_blank_line();

    put_int(a[0]);
    put_int(a[1]);
    put_blank_line();

    put_int(b[0][0]);
    put_int(b[0][1]);
    put_int(b[0][2]);
    put_int(b[1][0]);
    put_int(b[1][1]);
    put_int(b[1][2]);
}