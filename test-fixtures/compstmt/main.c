void put_int(int x);

int main(void) {
    int x, y;
    x = 2;
    y = 4;
    put_int(x);
    put_int(y);
    {
        int y;
        y = x - 1;
        put_int(x);
        put_int(y);
        x = x + y;
        put_int(x);
        put_int(y);
    }
    put_int(x);
    put_int(y);
    return 0;
}
