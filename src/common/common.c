#include "./common.h"

#include <stdlib.h>
#include <string.h>

int hash_integer(int* integer);
int comp_integer(int* a, int* b);
int hash_string(char* str);

BaseType t_integer = {
    .delete_object = free,
};
BaseType t_string = {
    .delete_object = free,
};

HashableType t_hashable_integer = {
    .hash_object = hash_integer,
    .compare_object = comp_integer,
    .delete_object = free,
};
HashableType t_hashable_string = {
    .hash_object = hash_string,
    .compare_object = strcmp,
    .delete_object = free,
};

int* new_integer(int value) {
    int* integer = malloc(sizeof(int));
    *integer = value;
    return integer;
}

int hash_integer(int* integer) {
    if (*integer >= 0)
        return *integer * 2;
    else
        return -(*integer) * 2 + 1;
}

int comp_integer(int* a, int* b) {
    return *a - *b;
}

char* new_string(char* str) {
    char* copied_str = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(copied_str, str);
    return copied_str;
}

int hash_string(char* str) {
    int a = 151, b = 587;
    int hash = 0;

    char* str_ptr = str;
    while (*str_ptr != '\0') {
        hash = (*str_ptr * a + hash) % b;
        str_ptr++;
    }

    return hash;
}
