#ifndef UMLCC_FREADER_H
#define UMLCC_FREADER_H

#include "./freaderret.h"

#include <stdio.h>

typedef struct FileReader {
    FILE* file_ptr;
} FileReader;

FileReader* new_freader(FILE* file_ptr);
FileReaderReturn* freader_read(FileReader* freader);
void delete_freader(FileReader* freader);

#endif
