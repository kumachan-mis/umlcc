#include "./freaderret.h"

#include <stdlib.h>

FileReaderReturn* new_freaderret(char* file_content) {
    FileReaderReturn* freaderret = malloc(sizeof(FileReaderReturn));
    freaderret->file_content = file_content;
    freaderret->err = NULL;
    return freaderret;
}

FileReaderReturn* new_freaderret_error(Error* err) {
    FileReaderReturn* freaderret = malloc(sizeof(FileReaderReturn));
    freaderret->file_content = NULL;
    freaderret->err = err;
    return freaderret;
}

void freaderret_assign(char** file_content_ptr, Error** err_ptr, FileReaderReturn* freaderret) {
    *file_content_ptr = freaderret->file_content;
    *err_ptr = freaderret->err;
    free(freaderret);
}
