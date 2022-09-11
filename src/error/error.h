#ifndef UMLCC_ERROR_H
#define UMLCC_ERROR_H

#include "../common/type.h"

typedef struct Error {
    char* message;
} Error;

extern BaseType t_error;

Error* new_error(char* format, ...);
Error* erorr_copy(Error* err);
void delete_error(Error* err);

#endif
