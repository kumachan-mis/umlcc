#ifndef UMLCC_ERROR_H
#define UMLCC_ERROR_H

typedef struct Error {
    char* message;
} Error;

Error* new_error(char* message);
void delete_error(Error* err);

#endif
