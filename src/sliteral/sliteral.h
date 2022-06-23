#ifndef UMLCC_SLITERAL_SLITERAL_H
#define UMLCC_SLITERAL_SLITERAL_H

#include "../common/type.h"

typedef struct StringLiteral {
    char* value;
    int size;
} StringLiteral;

extern BaseType t_sliteral;

StringLiteral* new_sliteral(char* value, int size);
StringLiteral* sliteral_copy(StringLiteral* sliteral);
StringLiteral* sliteral_zero_padding_copy(StringLiteral* sliteral, int size);
char* sliteral_display_string(StringLiteral* sliteral);
void delete_sliteral(StringLiteral* sliteral);

#endif
