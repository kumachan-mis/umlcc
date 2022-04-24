#include "./common.h"

#include <stdlib.h>
#include <string.h>


char* string_copy(char* str) {
    char* copied_str = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(copied_str, str);
    return copied_str;
}
