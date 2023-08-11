#ifndef UMLCC_X64_H_PRIVATE
#define UMLCC_X64_H_PRIVATE
#endif

#include "./data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

X64Data* new_int_x64data(X64DataType type, IntegerLiteral* iliteral) {
    X64Data* x64data = malloc(sizeof(X64Data));
    x64data->type = type;
    x64data->iliteral = iliteral;
    x64data->sliteral = NULL;
    return x64data;
}

X64Data* new_str_x64data(X64DataType type, StringLiteral* sliteral) {
    X64Data* x64data = malloc(sizeof(X64Data));
    x64data->type = type;
    x64data->iliteral = NULL;
    x64data->sliteral = sliteral;
    return x64data;
}

X64Data* x64data_copy(X64Data* x64data) {
    X64Data* copied_x64data = malloc(sizeof(X64Data));
    copied_x64data->type = x64data->type;
    copied_x64data->iliteral = NULL;
    if (x64data->iliteral != NULL) {
        copied_x64data->iliteral = iliteral_copy(x64data->iliteral);
    }
    copied_x64data->sliteral = NULL;
    if (x64data->sliteral != NULL) {
        copied_x64data->sliteral = sliteral_copy(x64data->sliteral);
    }
    return copied_x64data;
}

char* x64data_tostring(X64Data* x64data) {
    // TODO: fix to prevent buffer overflow
    char* data_str = malloc(200 * sizeof(char));
    char* display_str = NULL;

    switch (x64data->type) {
        case X64_DATA_BYTE:
            display_str = iliteral_display_string(x64data->iliteral);
            sprintf(data_str, "\t.byte\t%s\n", display_str);
            free(display_str);
            break;
        case X64_DATA_WORD:
            display_str = iliteral_display_string(x64data->iliteral);
            sprintf(data_str, "\t.word\t%s\n", display_str);
            free(display_str);
            break;
        case X64_DATA_LONG:
            display_str = iliteral_display_string(x64data->iliteral);
            sprintf(data_str, "\t.long\t%s\n", display_str);
            free(display_str);
            break;
        case X64_DATA_QUAD:
            display_str = iliteral_display_string(x64data->iliteral);
            sprintf(data_str, "\t.quad\t%s\n", display_str);
            free(display_str);
            break;
        case X64_DATA_ZERO:
            display_str = iliteral_display_string(x64data->iliteral);
            sprintf(data_str, "\t.zero\t%s\n", display_str);
            free(display_str);
            break;
        case X64_DATA_STR:
            display_str = sliteral_display_string(x64data->sliteral);
            sprintf(data_str, "\t.ascii\t%s\n", display_str);
            free(display_str);
            break;
    }

    data_str = realloc(data_str, (strlen(data_str) + 1) * sizeof(char));
    return data_str;
}

void delete_x64data(X64Data* x64data) {
    if (x64data->iliteral != NULL) {
        delete_iliteral(x64data->iliteral);
    }
    if (x64data->sliteral != NULL) {
        delete_sliteral(x64data->sliteral);
    }
    free(x64data);
}
