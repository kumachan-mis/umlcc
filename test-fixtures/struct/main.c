typedef struct Node {
    int value;
    struct Node* next;
} Node;

Node* new_node(int value);
void delete_node(Node* node);

void put_int(int x);
void put_blank_line(void);

struct {
    char x;
    int a[3];
} gtest = {12, 6, 10};

Node gnode = {5};

int main(void) {
    struct {
        char x;
        int a[3];
    } test = {1};

    put_int(gtest.x);
    put_int(gtest.a[0]);
    put_int(gtest.a[1]);
    put_int(gtest.a[2]);
    put_int(test.x);
    put_int(test.a[0]);
    put_int(test.a[1]);
    put_int(test.a[2]);
    put_blank_line();

    gtest.x = 7;
    gtest.a[0] = 11;
    test.a[2] = 3;
    test.a[1] = 2;

    put_int(gtest.x);
    put_int(gtest.a[0]);
    put_int(gtest.a[1]);
    put_int(gtest.a[2]);
    put_int(test.x);
    put_int(test.a[0]);
    put_int(test.a[1]);
    put_int(test.a[2]);
    put_blank_line();

    struct Coordinate {
        int x;
        int y;
        int z;
    };

    struct Coordinate point;

    point.y = 2;
    point.z = 5;
    point.x = 3;

    put_int(point.x);
    put_int(point.y);
    put_int(point.z);
    put_blank_line();

    Node* node = new_node(10);
    node->next = new_node(20);
    gnode.next = node;
    put_int(gnode.value);
    put_int(gnode.next->value);
    put_int(gnode.next->next->value);

    delete_node(node);

    return 0;
}
