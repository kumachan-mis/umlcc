#include "./errstr.h"

#include <stdlib.h>

ErrorableString* new_errstr(char* value) {
    ErrorableString* errstr = malloc(sizeof(ErrorableString));
    errstr->value = value;
    errstr->err = NULL;
    return errstr;
}

ErrorableString* new_errstr_error(Error* err) {
    ErrorableString* errstr = malloc(sizeof(ErrorableString));
    errstr->value = NULL;
    errstr->err = err;
    return errstr;
}

void errstr_assign(char** value_ptr, Error** err_ptr, ErrorableString* errstr) {
    *value_ptr = errstr->value;
    *err_ptr = errstr->err;
    free(errstr);
}
