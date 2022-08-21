#include "./errint.h"

#include <stdlib.h>

ErrorableInt* new_errint(int value) {
    ErrorableInt* errint = malloc(sizeof(ErrorableInt));
    errint->value = value;
    errint->err = NULL;
    return errint;
}

ErrorableInt* new_errint_error(Error* err) {
    ErrorableInt* errint = malloc(sizeof(ErrorableInt));
    errint->value = -1;
    errint->err = err;
    return errint;
}

void errint_assign(int* value_ptr, Error** err_ptr, ErrorableInt* errint) {
    *value_ptr = errint->value;
    *err_ptr = errint->err;
    free(errint);
}
