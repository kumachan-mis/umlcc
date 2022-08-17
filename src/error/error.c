#include "./error.h"

#include <stdlib.h>

Error* new_error(char* message) {
    Error* err = malloc(sizeof(Error));
    err->message = message;
    return err;
}

void delete_error(Error* err) {
    free(err->message);
    free(err);
}
