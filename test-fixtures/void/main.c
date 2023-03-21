void* malloc(int n);
void free(void* p);
void puts(char* s);

void put_int(int);
void put_blank_line(void);

void put_helloworld(void) {
    puts("Hello World");
}

int main(void) {
    put_helloworld();
    put_blank_line();

    typedef struct {
        int x;
    } VoidStruct;
    VoidStruct* s = malloc(4);
    s->x = 7;
    put_int(s->x);
    s->x = s->x * s->x;
    put_int(s->x);
    free(s);

    return 0;
}
