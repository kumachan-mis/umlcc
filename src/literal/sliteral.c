#include "./sliteral.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BaseType t_sliteral = {
    .copy_object = (void* (*)(void*))sliteral_copy,
    .delete_object = (void (*)(void*))delete_sliteral,
};

char ascii[92] = " !#$%&()*+,-./0123456789:;<=>@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";

StringLiteral* new_sliteral(char* value, int size) {
    StringLiteral* sliteral = malloc(sizeof(StringLiteral));
    sliteral->value = value;
    sliteral->size = size;
    return sliteral;
}

StringLiteral* sliteral_copy(StringLiteral* sliteral) {
    StringLiteral* copied_sliteral = malloc(sizeof(StringLiteral));
    copied_sliteral->value = malloc(sliteral->size * sizeof(char));
    memcpy(copied_sliteral->value, sliteral->value, sliteral->size);
    copied_sliteral->size = sliteral->size;
    return copied_sliteral;
}

StringLiteral* sliteral_zero_padding_copy(StringLiteral* sliteral, int size) {
    StringLiteral* copied_sliteral = malloc(sizeof(StringLiteral));
    copied_sliteral->value = malloc(size * sizeof(char));
    if (size > sliteral->size) {
        memset(copied_sliteral->value, 0, size);
        memcpy(copied_sliteral->value, sliteral->value, sliteral->size);
    } else {
        memcpy(copied_sliteral->value, sliteral->value, size);
    }
    copied_sliteral->size = size;
    return copied_sliteral;
}

char* sliteral_display_string(StringLiteral* sliteral) {
    char* display_string = malloc((4 * sliteral->size + 3) * sizeof(char));
    int len = 0;
    memset(display_string, 0, 4 * sliteral->size + 3);

    strcat(display_string, "\"");
    len++;

    for (int i = 0; i < sliteral->size; i++) {
        int c = sliteral->value[i];
        char* found = strchr(ascii, c);
        if (found != NULL && *found != '\0') {
            sprintf(display_string + len, "%c", c);
            len++;
            continue;
        }

        switch (c) {
            case '\'':
                sprintf(display_string + len, "\\\'");
                len += 2;
                break;
            case '\"':
                sprintf(display_string + len, "\\\"");
                len += 2;
                break;
            case '\?':
                sprintf(display_string + len, "\\\?");
                len += 2;
                break;
            case '\\':
                sprintf(display_string + len, "\\\\");
                len += 2;
                break;
            case '\a':
                sprintf(display_string + len, "\\a");
                len += 2;
                break;
            case '\b':
                sprintf(display_string + len, "\\b");
                len += 2;
                break;
            case '\f':
                sprintf(display_string + len, "\\f");
                len += 2;
                break;
            case '\n':
                sprintf(display_string + len, "\\n");
                len += 2;
                break;
            case '\r':
                sprintf(display_string + len, "\\r");
                len += 2;
                break;
            case '\t':
                sprintf(display_string + len, "\\t");
                len += 2;
                break;
            case '\v':
                sprintf(display_string + len, "\\v");
                len += 2;
                break;
            default:
                sprintf(display_string + len, "\\%03o", c);
                len += 4;
                break;
        }
    }

    strcat(display_string, "\"");
    len++;

    display_string = realloc(display_string, (len + 1) * sizeof(char));
    return display_string;
}

void delete_sliteral(StringLiteral* sliteral) {
    free(sliteral->value);
    free(sliteral);
}

char* sliteral_create_label(int sliteral_id) {
    // TODO: fix to prevent buffer overflow
    char* sliteral_name = malloc(50 * sizeof(char));
    sprintf(sliteral_name, ".SL%d", sliteral_id);
    sliteral_name = realloc(sliteral_name, (strlen(sliteral_name) + 1) * sizeof(char));
    return sliteral_name;
}
