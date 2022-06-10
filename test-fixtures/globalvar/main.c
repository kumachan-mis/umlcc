char c = 5;
char zc;

int i = 12;
int zi;

char ca[5] = {'d', 'e', 'f', '\0'};
char zca[5];

int ia[4] = {1, 2, 3};
int zia[4];

int put_int(int x);
int put_str(char* x);
int put_blank_line();

int main() {
    put_int(c);
    put_int(zc);
    put_blank_line();

    put_int(i);
    put_int(zi);
    put_blank_line();

    put_str(ca);
    put_str(zca);
    put_blank_line();

    put_str(ia[0]);
    put_str(ia[1]);
    put_str(ia[2]);
    put_str(ia[3]);
    put_str(zia[0]);
    put_str(zia[1]);
    put_str(zia[2]);
    put_str(zia[3]);

    return 0;
}
