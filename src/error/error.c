#include "./error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Error* new_error(char* format, ...) {
    Error* err = malloc(sizeof(Error));

    // TODO: fix to prevent buffer overflow
    char* message = malloc(200 * sizeof(char));
    va_list arg;
    va_start(arg, format);
    vsprintf(message, format, arg);
    va_end(arg);
    message = realloc(message, (strlen(message) + 1) * sizeof(char));

    err->message = message;

    return err;
}

void delete_error(Error* err) {
    free(err->message);
    free(err);
}
