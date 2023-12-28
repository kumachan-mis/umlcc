#include "./freader.h"

#include <stdlib.h>

FileReader* new_freader(FILE* file_ptr) {
    FileReader* freader = malloc(sizeof(FileReader));
    freader->file_ptr = file_ptr;
    return freader;
}

FileReaderReturn* freader_read(FileReader* freader) {
    char* file_content = NULL;
    Error* err = NULL;

    fseek(freader->file_ptr, 0, SEEK_END);
    long file_size = ftell(freader->file_ptr);
    if (file_size <= 0) {
        err = new_error("fail to read content of file");
        return new_freaderret_error(err);
    }

    rewind(freader->file_ptr);

    file_content = malloc(file_size + 1);
    file_size = fread(file_content, sizeof(char), file_size, freader->file_ptr);
    file_content[file_size] = '\0';

    file_content = realloc(file_content, file_size + 1);
    return new_freaderret(file_content);
}

void delete_freader(FileReader* freader) {
    fclose(freader->file_ptr);
    free(freader);
}
