typedef struct Node {
    struct Node* next;
    int value;
} Node;

Node* new_node(int value);
int delete_node(Node* node);

int put_int(int x);
int put_blank_line();

int main() {
    struct {
        int x;
        int a[3];
    } test = {1};
    put_int(test.x);
    put_int(test.a[0]);
    put_int(test.a[1]);
    put_int(test.a[2]);
    put_blank_line();

    test.a[2] = 3;
    test.a[1] = 2;

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
    put_int(node->value);
    put_int(node->next->value);

    delete_node(node);

    return 0;
}
