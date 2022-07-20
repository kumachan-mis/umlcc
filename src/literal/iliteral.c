#include "./iliteral.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BaseType t_iliteral = {
    .copy_object = (void* (*)(void*))iliteral_copy,
    .delete_object = (void (*)(void*))delete_iliteral,
};

IntegerLiteral* new_signed_iliteral(IntegerLiteralType type, long long value) {
    IntegerLiteral* iliteral = malloc(sizeof(IntegerLiteral));
    iliteral->type = type;
    iliteral->is_unsigned = 0;
    iliteral->signed_value = value;
    iliteral->unsigned_value = 0ULL;
    return iliteral;
}

IntegerLiteral* new_unsigned_iliteral(IntegerLiteralType type, unsigned long long value) {
    IntegerLiteral* iliteral = malloc(sizeof(IntegerLiteral));
    iliteral->type = type;
    iliteral->is_unsigned = 1;
    iliteral->signed_value = -1LL;
    iliteral->unsigned_value = value;
    return iliteral;
}

IntegerLiteral* iliteral_copy(IntegerLiteral* iliteral) {
    IntegerLiteral* copied_iliteral = malloc(sizeof(IntegerLiteral));
    copied_iliteral->type = iliteral->type;
    copied_iliteral->is_unsigned = iliteral->is_unsigned;
    copied_iliteral->signed_value = iliteral->signed_value;
    copied_iliteral->unsigned_value = iliteral->unsigned_value;
    return copied_iliteral;
}

char* iliteral_display_string(IntegerLiteral* iliteral) {
    char* display_string = malloc(50 * sizeof(char));
    if (!iliteral->is_unsigned) {
        sprintf(display_string, "%lld", iliteral->signed_value);
    } else {
        sprintf(display_string, "%llu", iliteral->unsigned_value);
    }

    display_string = realloc(display_string, (strlen(display_string) + 1) * sizeof(char));
    return display_string;
}

void delete_iliteral(IntegerLiteral* iliteral) {
    free(iliteral);
}
