#include "./srt.h"
#include "../common/util.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

BaseType t_srt = {
    .copy_object = (void* (*)(void*))srt_copy,
    .delete_object = (void (*)(void*))delete_srt,
};

Srt* new_srt(SrtType type, int num_children, ...) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = NULL;
    srt->ident_name = NULL;
    srt->value_int = -1;
    srt->value_str = NULL;
    srt->children = new_vector(&t_srt);

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(srt->children, va_arg(children, Srt*));
    }
    va_end(children);

    return srt;
}

Srt* new_dtyped_srt(SrtType type, Dtype* dtype, int num_children, ...) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = NULL;
    srt->value_int = -1;
    srt->value_str = NULL;
    srt->children = new_vector(&t_srt);

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(srt->children, va_arg(children, Srt*));
    }
    va_end(children);

    return srt;
}

Srt* new_identifier_srt(SrtType type, Dtype* dtype, char* ident_name) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = ident_name;
    srt->value_int = -1;
    srt->value_str = NULL;
    srt->children = new_vector(&t_srt);
    return srt;
}

Srt* new_integer_srt(SrtType type, Dtype* dtype, int value) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = NULL;
    srt->value_int = value;
    srt->value_str = NULL;
    srt->children = new_vector(&t_srt);
    return srt;
}

Srt* new_string_literal_srt(SrtType type, Dtype* dtype, char* value) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = NULL;
    srt->value_int = -1;
    srt->value_str = value;
    srt->children = new_vector(&t_srt);
    return srt;
}

Srt* srt_copy(Srt* srt) {
    Srt* copied_srt = malloc(sizeof(Srt));
    copied_srt->dtype = NULL;
    if (srt->dtype != NULL) copied_srt->dtype = dtype_copy(srt->dtype);
    copied_srt->ident_name = NULL;
    if (srt->ident_name != NULL) copied_srt->ident_name = new_string(srt->ident_name);
    copied_srt->value_int = srt->value_int;
    copied_srt->value_str = NULL;
    if (srt->value_str != NULL) {
        int size = strlen(srt->value_str) + 1;
        if (srt->dtype != NULL && srt->dtype->type == DTYPE_ARRAY) size = srt->dtype->array->size;
        copied_srt->value_str = copy_memory(srt->value_str, size * sizeof(char));
    }
    copied_srt->children = vector_copy(srt->children);
    return copied_srt;
}

void delete_srt(Srt* srt) {
    if (srt->dtype != NULL) delete_dtype(srt->dtype);
    if (srt->ident_name != NULL) free(srt->ident_name);
    delete_vector(srt->children);
    free(srt);
}
