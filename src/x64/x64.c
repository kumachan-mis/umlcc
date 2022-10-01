#ifndef UMLCC_X64_H_PRIVATE
#define UMLCC_X64_H_PRIVATE
#endif

#include "./x64.h"

#include <stdlib.h>

BaseType t_x64 = {
    .copy_object = (void* (*)(void*))x64_copy,
    .delete_object = (void (*)(void*))delete_x64,
};

X64* new_inst_x64(X64InstType type, X64Ope* src, X64Ope* dst) {
    X64* x64 = malloc(sizeof(X64));
    x64->type = X64_INST;
    x64->inst = new_x64inst(type, src, dst);
    x64->data = NULL;
    x64->label = NULL;
    return x64;
}

X64* new_int_data_x64(X64DataType type, IntegerLiteral* iliteral) {
    X64* x64 = malloc(sizeof(X64));
    x64->type = X64_DATA;
    x64->inst = NULL;
    x64->data = new_int_x64data(type, iliteral);
    x64->label = NULL;
    return x64;
}

X64* new_str_data_x64(X64DataType type, StringLiteral* sliteral) {
    X64* x64 = malloc(sizeof(X64));
    x64->type = X64_DATA;
    x64->inst = NULL;
    x64->data = new_str_x64data(type, sliteral);
    x64->label = NULL;
    return x64;
}

X64* new_label_x64(X64LabelType type, X64Visibility visibility, char* name) {
    X64* x64 = malloc(sizeof(X64));
    x64->type = X64_LABEL;
    x64->inst = NULL;
    x64->data = NULL;
    x64->label = new_x64label(type, visibility, name);
    return x64;
}

X64* x64_copy(X64* x64) {
    X64* copied_x64 = malloc(sizeof(X64));
    copied_x64->type = x64->type;

    copied_x64->inst = NULL;
    if (x64->inst != NULL) copied_x64->inst = x64inst_copy(x64->inst);
    copied_x64->data = NULL;
    if (x64->data != NULL) copied_x64->data = x64data_copy(x64->data);
    copied_x64->label = NULL;
    if (x64->label != NULL) copied_x64->label = x64label_copy(x64->label);

    return copied_x64;
}

char* x64_tostring(X64* x64) {
    switch (x64->type) {
        case X64_INST:
            return x64inst_tostring(x64->inst);
        case X64_DATA:
            return x64data_tostring(x64->data);
        case X64_LABEL:
            return x64label_tostring(x64->label);
        default:
            return NULL;
    }
}

void delete_x64(X64* x64) {
    if (x64->inst != NULL) delete_x64inst(x64->inst);
    if (x64->data != NULL) delete_x64data(x64->data);
    if (x64->label != NULL) delete_x64label(x64->label);
    free(x64);
}
