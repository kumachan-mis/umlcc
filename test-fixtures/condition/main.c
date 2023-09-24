int strcmp(char* s1, char* s2);

void put_int(int x);
void put_str(char* s);
void put_blank_line(void);

int max_int(int a, int b) {
    return a >= b ? a : b;
}

int min_int(int a, int b) {
    return a <= b ? a : b;
}

typedef enum IntegerLiteralType {
    INTEGER_INT,
    INTEGER_UNSIGNED_INT,
    INTEGER_LONG,
    INTEGER_UNSIGNED_LONG,
    INTEGER_LONGLONG,
    INTEGER_UNSIGNED_LONGLONG,
    INTEGER_UNKNOWN
} IntegerLiteralType;

IntegerLiteralType get_iliteral_type(char rank[10], int is_unsigned) {
    if (strcmp(rank, "int") == 0) {
        return is_unsigned ? INTEGER_UNSIGNED_INT : INTEGER_INT;
    } else if (strcmp(rank, "long") == 0) {
        return is_unsigned ? INTEGER_UNSIGNED_LONG : INTEGER_LONG;
    } else if (strcmp(rank, "long long") == 0) {
        return is_unsigned ? INTEGER_UNSIGNED_LONGLONG : INTEGER_LONGLONG;
    }
    return INTEGER_UNKNOWN;
}

int main(void) {
    put_int(max_int(1, 2));
    put_int(max_int(5, 2));
    put_int(min_int(1, 2));
    put_int(min_int(5, 2));
    put_blank_line();

    put_int(get_iliteral_type("long", 0));
    put_int(get_iliteral_type("long", 1));
    put_int(get_iliteral_type("int", 0));
    put_int(get_iliteral_type("int", 1));
    put_int(get_iliteral_type("long long", 0));
    put_int(get_iliteral_type("long long", 1));
    put_int(get_iliteral_type("unknown", 0));
    put_int(get_iliteral_type("unknown", 1));
    put_blank_line();

    return 0;
}