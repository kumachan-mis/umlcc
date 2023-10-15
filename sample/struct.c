int put_int(int x);

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
struct Child gc = {1};

int main(void) {
    struct Struct l = {1};
    struct Child lc = {1};

    put_int(lc.x);
    put_int(lc.y);

    put_int(l.a.x);
    put_int(l.a.y);
    put_int(l.b.x);
    put_int(l.b.y);

    put_int(gc.x);
    put_int(gc.y);

    put_int(g.a.x);
    put_int(g.a.y);
    put_int(g.b.x);
    put_int(g.b.y);
}
