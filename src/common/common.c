#include "./common.h"

#include <stdlib.h>
#include <string.h>

int* new_integer(int value) {
    int* integer = malloc(sizeof(int));
    *integer = value;
    return integer;
}

void delete_integer(int* integer) {
    free(integer);
}

int integer_hash(int* integer, int max) {
    return *integer % max;
}

int integer_comp(int* a, int* b) {
    return *a - *b;
}

char* new_string(char* str) {
    char* copied_str = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(copied_str, str);
    return copied_str;
}

void delete_str(char* str) {
    free(str);
}

int str_hash(char* str, int max) {
    int a = 151, b = 587;
    int hash = 0;

    char* str_ptr = str;
    while (*str_ptr != '\0') {
        hash = (*str_ptr * a + hash) % b;
        str_ptr++;
    }

    return hash % max;
}

int str_comp(char* a, char* b) {
    return strcmp(a, b);
}
