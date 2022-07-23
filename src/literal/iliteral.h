#ifndef UMLCC_LITERAL_ILITERAL_H
#define UMLCC_LITERAL_ILITERAL_H

#include "../common/type.h"

typedef enum IntegerLiteralType {
    INTEGER_INT,
    INTEGER_LONG,
    INTEGER_LONGLONG,
} IntegerLiteralType;

typedef struct IntegerLiteral {
    IntegerLiteralType type;
    int is_unsigned;
    long long signed_value;
    unsigned long long unsigned_value;
} IntegerLiteral;

extern BaseType t_iliteral;

IntegerLiteral* new_signed_iliteral(IntegerLiteralType type, long long value);
IntegerLiteral* new_unsigned_iliteral(IntegerLiteralType type, unsigned long long value);
IntegerLiteral* iliteral_copy(IntegerLiteral* iliteral);
char* iliteral_display_string(IntegerLiteral* iliteral);
void delete_iliteral(IntegerLiteral* iliteral);

#endif
