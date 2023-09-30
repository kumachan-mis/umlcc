void put_int(int x);
void put_str(char* str);

int main(void) {

    while (0) {
        put_str("ERROR: while loop should not be executed");
    }

    int x = 0;
    while (x < 5) {
        put_int(x);
        x = x + 1;
    }

    int y = 0;
    int z = 1;
    while (y < 10) {
        z = 2 * z;
        y = y + 1;
    }
    put_int(z);

    return 0;
}
