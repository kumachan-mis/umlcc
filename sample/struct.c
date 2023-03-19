struct Child {
    char x;
    int y;
    char z;
};

struct Struct {
    struct Child a;
    struct Child b;
};

struct Struct g = {1};

int main(void) {
    struct Struct l = {1};
}
