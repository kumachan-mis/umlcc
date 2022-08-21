#ifndef UMLCC_ERROR_ERRINT_H
#define UMLCC_ERROR_ERRINT_H

#include "./error.h"

typedef struct ErrorableInt {
    int value;
    Error* err;
} ErrorableInt;

ErrorableInt* new_errint(int value);
ErrorableInt* new_errint_error(Error* err);
void errint_assign(int* value_ptr, Error** err_ptr, ErrorableInt* errint);

#endif
