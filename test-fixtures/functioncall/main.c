int put_7ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7);
int put_8ints(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8);
int put_blank_line();

int main() {
    put_7ints(1, 3, 5, 7, 9, 11, 13);
    put_blank_line();
    put_8ints(2, 4, 8, 16, 32, 64, 128, 256);
}