#include "./error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BaseType t_error = {
    .copy_object = (void* (*)(void*))erorr_copy,
    .delete_object = (void (*)(void*))delete_error,
};

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

Error* erorr_copy(Error* err) {
    Error* copied_err = malloc(sizeof(Error));
    copied_err->message = new_string(err->message);
    return copied_err;
}

void delete_error(Error* err) {
    free(err->message);
    free(err);
}
