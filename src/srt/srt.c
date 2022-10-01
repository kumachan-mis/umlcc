#include "./srt.h"

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
    srt->iliteral = NULL;
    srt->sliteral = NULL;
    srt->children = new_vector(&t_srt);

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(srt->children, va_arg(children, Srt*));
    }
    va_end(children);

    return srt;
}

Srt* new_dtyped_srt(SrtType type, DType* dtype, int num_children, ...) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = NULL;
    srt->iliteral = NULL;
    srt->sliteral = NULL;
    srt->children = new_vector(&t_srt);

    va_list children;
    va_start(children, num_children);
    for (int i = 0; i < num_children; i++) {
        vector_push(srt->children, va_arg(children, Srt*));
    }
    va_end(children);

    return srt;
}

Srt* new_identifier_srt(SrtType type, DType* dtype, char* ident_name) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = ident_name;
    srt->iliteral = NULL;
    srt->sliteral = NULL;
    srt->children = new_vector(&t_srt);
    return srt;
}

Srt* new_iliteral_srt(SrtType type, DType* dtype, IntegerLiteral* iliteral) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = NULL;
    srt->iliteral = iliteral;
    srt->sliteral = NULL;
    srt->children = new_vector(&t_srt);
    return srt;
}

Srt* new_sliteral_srt(SrtType type, DType* dtype, StringLiteral* sliteral) {
    Srt* srt = malloc(sizeof(Srt));
    srt->type = type;
    srt->dtype = dtype;
    srt->ident_name = NULL;
    srt->iliteral = NULL;
    srt->sliteral = sliteral;
    srt->children = new_vector(&t_srt);
    return srt;
}

Srt* srt_copy(Srt* srt) {
    Srt* copied_srt = malloc(sizeof(Srt));
    copied_srt->type = srt->type;
    copied_srt->dtype = NULL;
    if (srt->dtype != NULL) copied_srt->dtype = dtype_copy(srt->dtype);
    copied_srt->ident_name = NULL;
    if (srt->ident_name != NULL) copied_srt->ident_name = new_string(srt->ident_name);
    copied_srt->iliteral = NULL;
    if (srt->iliteral != NULL) copied_srt->iliteral = iliteral_copy(srt->iliteral);
    copied_srt->sliteral = NULL;
    if (srt->sliteral != NULL) copied_srt->sliteral = sliteral_copy(srt->sliteral);
    copied_srt->children = vector_copy(srt->children);
    return copied_srt;
}

int srt_islvalue(Srt* srt) {
    if (srt->dtype == NULL || !dtype_isobject(srt->dtype)) return 0;
    return srt->type == SRT_INDIR_EXPR || srt->type == SRT_IDENT_EXPR || srt->type == SRT_STRING_EXPR;
}

int srt_ismodifiable(Srt* srt) {
    if (srt->dtype == NULL || !dtype_isobject(srt->dtype) || srt->dtype->type == DTYPE_ARRAY) return 0;
    return srt->type == SRT_INDIR_EXPR || srt->type == SRT_IDENT_EXPR;
}

void delete_srt(Srt* srt) {
    if (srt->dtype != NULL) delete_dtype(srt->dtype);
    if (srt->ident_name != NULL) free(srt->ident_name);
    if (srt->iliteral != NULL) delete_iliteral(srt->iliteral);
    if (srt->sliteral != NULL) delete_sliteral(srt->sliteral);
    delete_vector(srt->children);
    free(srt);
}
