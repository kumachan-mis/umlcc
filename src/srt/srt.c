#include "./srt.h"

#include <stdarg.h>
#include <stdlib.h>

BaseType t_srt = {
    .copy_object = srt_copy,
    .delete_object = delete_srt,
};

Srt* new_srt(SrtType type, int num_children, ...) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->ctype = NULL;
    srt->ident_name = NULL;
    srt->value_int = -1;
    srt->children = new_vector(&t_srt);

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(srt->children, va_arg(children, Srt*));
    }
    va_end(children);

    return srt;
}

Srt* new_ctyped_srt(SrtType type, CType* ctype, int num_children, ...) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->ctype = ctype;
    srt->ident_name = NULL;
    srt->value_int = -1;
    srt->children = new_vector(&t_srt);

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(srt->children, va_arg(children, Srt*));
    }
    va_end(children);

    return srt;
}

Srt* new_identifier_srt(SrtType type, CType* ctype, char* ident_name) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->ctype = ctype;
    srt->ident_name = ident_name;
    srt->value_int = -1;
    srt->children = new_vector(&t_srt);

    return srt;
}

Srt* new_integer_srt(SrtType type, int value) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->ctype = new_integer_ctype();
    srt->ident_name = NULL;
    srt->value_int = value;
    srt->children = new_vector(&t_srt);

    return srt;
}

void delete_srt(Srt* srt) {
    if (srt->ctype != NULL) delete_ctype(srt->ctype);
    if (srt->ident_name != NULL) free(srt->ident_name);
    delete_vector(srt->children);
    free(srt);
}

Srt* srt_copy(Srt* srt) {
    Srt* copied_srt = malloc(sizeof(Srt));
    copied_srt->ctype = NULL;
    if (srt->ctype != NULL) copied_srt->ctype = ctype_copy(srt->ctype);
    copied_srt->ident_name = NULL;
    if (srt->ident_name != NULL) copied_srt->ident_name = new_string(srt->ident_name);
    copied_srt->children = vector_copy(srt->children);
    return copied_srt;
}
