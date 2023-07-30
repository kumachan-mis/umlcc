#ifndef UMLCC_ERROR_ERRSTR_H
#define UMLCC_ERROR_ERRSTR_H

#include "./error.h"

typedef struct ErrorableString {
    char* value;
    Error* err;
} ErrorableString;

ErrorableString* new_errstr(char* value);
ErrorableString* new_errstr_error(Error* err);
void errstr_assign(char** value_ptr, Error** err_ptr, ErrorableString* errstr);

#endif
