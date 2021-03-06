#include "./type.h"

#include <stdlib.h>
#include <string.h>

int* copy_integer(int* integer);
int hash_integer(int* integer);
int comp_integer(int* a, int* b);
int hash_string(char* str);

BaseType t_integer = {
    .copy_object = (void* (*)(void*))copy_integer,
    .delete_object = (void (*)(void*))free,
};
BaseType t_string = {
    .copy_object = (void* (*)(void*))new_string,
    .delete_object = (void (*)(void*))free,
};

HashableType t_hashable_integer = {
    .copy_object = (void* (*)(void*))copy_integer,
    .delete_object = (void (*)(void*))free,
    .hash_object = (int (*)(void*))hash_integer,
    .compare_object = (int (*)(void*, void*))comp_integer,
};
HashableType t_hashable_string = {
    .copy_object = (void* (*)(void*))new_string,
    .delete_object = (void (*)(void*))free,
    .hash_object = (int (*)(void*))hash_string,
    .compare_object = (int (*)(void*, void*))strcmp,
};

int* new_integer(int value) {
    int* integer = malloc(sizeof(int));
    *integer = value;
    return integer;
}

int* copy_integer(int* integer) {
    int* copied_integer = malloc(sizeof(int));
    *copied_integer = *integer;
    return copied_integer;
}

int hash_integer(int* integer) {
    if (*integer >= 0) {
        return *integer * 2;
    } else {
        return -(*integer) * 2 + 1;
    }
}

int comp_integer(int* a, int* b) {
    return *a - *b;
}

char* new_string(char* str) {
    char* string = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(string, str);
    return string;
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
