#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ascii[92] =
    " !#$%&()*+,-./0123456789:;<=>@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";

void swap_ptr(void** a, void** b) {
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}

char* copy_charmem(char* charmem, int size) {
    char* dst = malloc(size * sizeof(char));
    memcpy(dst, charmem, size);
    return dst;
}

char* copy_charmem_zero_padding(char* charmem, int charmem_size, int copy_size) {
    char* dst = malloc(copy_size * sizeof(char));
    memset(dst, 0, copy_size);
    memcpy(dst, charmem, charmem_size);
    return dst;
}

char* charmem_tostring(char* charmem, int size) {
    char* charmem_str = malloc((4 * size + 3) * sizeof(char));
    int len = 0;
    memset(charmem_str, 0, 4 * size + 3);

    strcat(charmem_str, "\"");
    len++;

    for (int i = 0; i < size; i++) {
        int c = charmem[i];
        char* found = strchr(ascii, c);
        if (found != NULL && *found != '\0') {
            sprintf(charmem_str + len, "%c", c);
            len++;
            continue;
        }

        switch (c) {
            case '\'':
                sprintf(charmem_str + len, "\\\'");
                len += 2;
                break;
            case '\"':
                sprintf(charmem_str + len, "\\\"");
                len += 2;
                break;
            case '\?':
                sprintf(charmem_str + len, "\\\?");
                len += 2;
                break;
            case '\\':
                sprintf(charmem_str + len, "\\\\");
                len += 2;
                break;
            case '\a':
                sprintf(charmem_str + len, "\\a");
                len += 2;
                break;
            case '\b':
                sprintf(charmem_str + len, "\\b");
                len += 2;
                break;
            case '\f':
                sprintf(charmem_str + len, "\\f");
                len += 2;
                break;
            case '\n':
                sprintf(charmem_str + len, "\\n");
                len += 2;
                break;
            case '\r':
                sprintf(charmem_str + len, "\\r");
                len += 2;
                break;
            case '\t':
                sprintf(charmem_str + len, "\\t");
                len += 2;
                break;
            case '\v':
                sprintf(charmem_str + len, "\\v");
                len += 2;
                break;
            default:
                sprintf(charmem_str + len, "\\%03o", c);
                len += 4;
                break;
        }
    }

    strcat(charmem_str, "\"");
    len++;

    charmem_str = realloc(charmem_str, (len + 1) * sizeof(char));
    return charmem_str;
}