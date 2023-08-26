#include "./iliteral.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BaseType t_iliteral = {
    .copy_object = (void* (*)(void*))iliteral_copy,
    .delete_object = (void (*)(void*))delete_iliteral,
};

IntegerLiteral* new_base_iliteral(IntegerLiteralType type);

IntegerLiteral* new_signed_iliteral(IntegerLiteralType type, long long value) {
    IntegerLiteral* iliteral = new_base_iliteral(type);
    iliteral->signed_value = value;
    return iliteral;
}

IntegerLiteral* new_unsigned_iliteral(IntegerLiteralType type, unsigned long long value) {
    IntegerLiteral* iliteral = new_base_iliteral(type);
    iliteral->unsigned_value = value;
    return iliteral;
}

IntegerLiteral* new_base_iliteral(IntegerLiteralType type) {
    IntegerLiteral* iliteral = malloc(sizeof(IntegerLiteral));
    iliteral->type = type;
    iliteral->signed_value = -1ll;
    iliteral->unsigned_value = 0ull;
    return iliteral;
}

IntegerLiteral* iliteral_copy(IntegerLiteral* iliteral) {
    IntegerLiteral* copied_iliteral = malloc(sizeof(IntegerLiteral));
    copied_iliteral->type = iliteral->type;
    copied_iliteral->signed_value = iliteral->signed_value;
    copied_iliteral->unsigned_value = iliteral->unsigned_value;
    return copied_iliteral;
}

int iliteral_isunsigned(IntegerLiteral* iliteral) {
    return iliteral_type_isunsigned(iliteral->type);
}

char* iliteral_display_string(IntegerLiteral* iliteral) {
    char* display_string = malloc(50 * sizeof(char));
    if (iliteral_isunsigned(iliteral)) {
        sprintf(display_string, "%llu", iliteral->unsigned_value);
    } else {
        sprintf(display_string, "%lld", iliteral->signed_value);
    }

    display_string = realloc(display_string, (strlen(display_string) + 1) * sizeof(char));
    return display_string;
}

void delete_iliteral(IntegerLiteral* iliteral) {
    free(iliteral);
}

int iliteral_type_isunsigned(IntegerLiteralType type) {
    return type == INTEGER_UNSIGNED_INT || type == INTEGER_UNSIGNED_LONG || type == INTEGER_UNSIGNED_LONGLONG;
}

IntegerLiteralType iliteral_type_get(int scalar_rank, int is_unsigned) {
    if (scalar_rank < 3) {
        return is_unsigned ? INTEGER_UNSIGNED_INT : INTEGER_INT;
    } else if (scalar_rank == 3) {
        return is_unsigned ? INTEGER_UNSIGNED_LONG : INTEGER_LONG;
    } else {
        return is_unsigned ? INTEGER_UNSIGNED_LONGLONG : INTEGER_LONGLONG;
    }
}
