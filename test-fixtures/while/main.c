void put_int(int x);
void put_str(char* str);
void put_blank_line(void);

int main(void) {

    while (0) {
        put_str("ERROR: while loop should not be executed");
    }

    int x = 0;
    while (x < 5) {
        put_int(x);
        x++;
    }
    put_blank_line();

    int y = 0;
    int z = 1;
    while (y < 10) {
        z = 2 * z;
        y++;
        put_int(z);
    }
    put_blank_line();

    y = 0;
    while (1) {
        if (y > 10) {
            break;
        }
        put_int(y);
        y += 2;
    }
    put_blank_line();

    y = 0;
    while (y < 5) {
        if (y % 2 == 0) {
            y++;
            continue;
        }
        put_int(y);
        y++;
    }
    put_blank_line();

    return 0;
}
