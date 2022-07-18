#include "./data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

X64Data* new_imm_x64data(X64DataType type, int value) {
    X64Data* x64data = malloc(sizeof(X64Data));
    x64data->type = type;
    x64data->imm_value = value;
    x64data->sliteral = NULL;
    return x64data;
}

X64Data* new_str_x64data(X64DataType type, StringLiteral* sliteral) {
    X64Data* x64data = malloc(sizeof(X64Data));
    x64data->type = type;
    x64data->imm_value = -1;
    x64data->sliteral = sliteral;
    return x64data;
}

X64Data* x64data_copy(X64Data* x64data) {
    X64Data* copied_x64data = malloc(sizeof(X64Data));
    copied_x64data->type = x64data->type;
    copied_x64data->imm_value = x64data->imm_value;
    copied_x64data->sliteral = NULL;
    if (x64data->sliteral != NULL) copied_x64data->sliteral = sliteral_copy(x64data->sliteral);
    return copied_x64data;
}

char* x64data_tostring(X64Data* x64data) {
    char* data_str = malloc(200 * sizeof(char));

    switch (x64data->type) {
        case X64_DATA_BYTE:
            sprintf(data_str, "\t.byte\t%d\n", x64data->imm_value);
            break;
        case X64_DATA_WORD:
            sprintf(data_str, "\t.word\t%d\n", x64data->imm_value);
            break;
        case X64_DATA_LONG:
            sprintf(data_str, "\t.long\t%d\n", x64data->imm_value);
            break;
        case X64_DATA_QUAD:
            sprintf(data_str, "\t.quad\t%d\n", x64data->imm_value);
            break;
        case X64_DATA_ZERO:
            sprintf(data_str, "\t.zero\t%d\n", x64data->imm_value);
            break;
        case X64_DATA_STR: {
            char* display_str = sliteral_display_string(x64data->sliteral);
            sprintf(data_str, "\t.ascii\t%s\n", display_str);
            free(display_str);
            break;
        }
    }

    data_str = realloc(data_str, (strlen(data_str) + 1) * sizeof(char));
    return data_str;
}

void delete_x64data(X64Data* x64data) {
    if (x64data->sliteral != NULL) delete_sliteral(x64data->sliteral);
    free(x64data);
}
