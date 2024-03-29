void put_int(int x);
void put_str(char* str);
void put_blank_line(void);

int main(void) {
    for (; 0;) {
        put_str("ERROR: for loop should not be executed");
    }

    for (int i = 0; i < 5; i++) {
        put_int(i);
    }
    put_blank_line();

    int i = 0, y = 0, z = 1;
    for (i = 0; i < 11; i++) {
        z = 2 * z;
        put_int(z);
    }
    put_blank_line();

    for (int i = 0;; i += 2) {
        if (i > 10) {
            break;
        }
        put_int(i);
    }
    put_blank_line();

    for (int i = 0; i < 5; i++) {
        if (i == 3) {
            continue;
        }
        put_int(i);
    }
    put_blank_line();

    return 0;
}
