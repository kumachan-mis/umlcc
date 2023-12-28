#ifndef UMLCC_FREADER_FREADERRET_H
#define UMLCC_FREADER_FREADERRET_H

#include "../error/error.h"

typedef struct FileReaderReturn {
    char* file_content;
    Error* err;
} FileReaderReturn;

FileReaderReturn* new_freaderret(char* file_content);
FileReaderReturn* new_freaderret_error(Error* err);
void freaderret_assign(char** file_content_ptr, Error** err_ptr, FileReaderReturn* freaderret);

#endif
