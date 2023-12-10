void put_int(int x);
void put_str(char* s);
void put_blank_line(void);

typedef enum IntegerType {
    INT,
    UNSIGNED_INT,
    LONG,
    UNSIGNED_LONG,
    LONG_LONG,
    UNSIGNED_LONG_LONG,
} IntegerType;

int get_integer_unsigned(IntegerType type) {
    switch (type) {
        case UNSIGNED_INT:
        case UNSIGNED_LONG:
        case UNSIGNED_LONG_LONG:
            return 1;
        default:
            return 0;
    }
}

void put_integer_size(IntegerType type) {
    switch (type) {
        case INT:
        case UNSIGNED_INT:
            put_int(4);
            break;
        case LONG:
        case UNSIGNED_LONG:
            put_int(8);
            break;
        case LONG_LONG:
        case UNSIGNED_LONG_LONG:
            put_int(8);
            break;
    }
}

void put_integer_type_name(IntegerType type) {
    switch (type) {
        case INT:
            put_str("int");
            break;
        case UNSIGNED_INT:
            put_str("unsigned int");
            break;
        case LONG:
            put_str("long");
            break;
        case UNSIGNED_LONG:
            put_str("unsigned long");
            break;
        case LONG_LONG:
            put_str("long long");
            break;
        case UNSIGNED_LONG_LONG:
            put_str("unsigned long long");
            break;
    }
}

int various_integer_literal(int x) {
    switch (x) {
        case 3:
            return 3;
        default:
            return -1;
        case 1L:
            return 10;
        case 2LL:
            return 200;
    }
}

int main(void) {
    put_int(get_integer_unsigned(INT));
    put_int(get_integer_unsigned(UNSIGNED_INT));
    put_int(get_integer_unsigned(LONG));
    put_int(get_integer_unsigned(UNSIGNED_LONG));
    put_int(get_integer_unsigned(LONG_LONG));
    put_int(get_integer_unsigned(UNSIGNED_LONG_LONG));
    put_blank_line();

    put_integer_size(INT);
    put_integer_size(UNSIGNED_INT);
    put_integer_size(LONG);
    put_integer_size(UNSIGNED_LONG);
    put_integer_size(LONG_LONG);
    put_integer_size(UNSIGNED_LONG_LONG);
    put_blank_line();

    put_integer_type_name(INT);
    put_integer_type_name(UNSIGNED_INT);
    put_integer_type_name(LONG);
    put_integer_type_name(UNSIGNED_LONG);
    put_integer_type_name(LONG_LONG);
    put_integer_type_name(UNSIGNED_LONG_LONG);
    put_blank_line();

    put_int(various_integer_literal(0));
    put_int(various_integer_literal(1));
    put_int(various_integer_literal(2));
    put_int(various_integer_literal(3));
    put_blank_line();

    return 0;
}
