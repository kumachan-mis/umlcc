#include "./type.h"

#include <stdlib.h>


CType* new_integer_ctype() {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_INT;
    ctype->pointer = NULL;
    ctype->function = NULL;
    return ctype;
}

CType* new_pointer_ctype(CType* to_ctype) {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_POINTER;
    ctype->pointer = new_cpointer(to_ctype);
    ctype->function = NULL;
    return ctype;
}

CType* new_socket_pointer_ctype() {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_POINTER;
    ctype->pointer = new_socket_cpointer();
    ctype->function = NULL;
    return ctype;
}

CType* new_function_ctype(Vector* params, CType* return_ctype) {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_FUNCUCTION;
    ctype->pointer = NULL;
    ctype->function = new_cfunction(params, return_ctype);
    return ctype;
}

CType* new_socket_function_ctype(Vector* params) {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_FUNCUCTION;
    ctype->pointer = NULL;
    ctype->function = new_socket_cfunction(params);
    return ctype;
}

CType* ctype_copy(CType* ctype) {
    CType* copied_ctype = malloc(sizeof(CType));
    copied_ctype->type = ctype->type;

    copied_ctype->pointer = NULL;
    if (ctype->pointer != NULL) {
        copied_ctype->pointer = cpointer_copy(ctype->pointer);
    }
    copied_ctype->function = NULL;
    if (ctype->function != NULL) {
        copied_ctype->function = cfunction_copy(ctype->function);
    }
    return copied_ctype;
}

CType* ctype_connect(CType* socket, CType* plug) {
    if (socket == NULL) return plug;

    CType* fragment = socket;
    while (1) {
        switch (fragment->type) {
            case CTYPE_INT:
                return socket;
            case CTYPE_POINTER: {
                CType* next = cpointer_next(fragment->pointer);
                if (next == NULL) {
                    fragment->pointer = cpointer_connect(fragment->pointer, plug);
                    return socket;
                }
                fragment = next;
                break;
            }
            case CTYPE_FUNCUCTION: {
                CType* next = cfunction_next(fragment->function);
                if (next == NULL) {
                    fragment->function = cfunction_connect(fragment->function, plug);
                    return socket;
                }
                fragment = next;
                break;
            }
        }
    }
}

int ctype_size(CType* ctype) {
    switch (ctype->type) {
        case CTYPE_INT:
            return 4;
        case CTYPE_POINTER:
            return 8;
        case CTYPE_FUNCUCTION:
            return 0;
        default:
            return -1;
    }
}

void delete_ctype(CType* ctype) {
     if (ctype->pointer != NULL) {
        delete_cpointer(ctype->pointer);
    }
    if (ctype->function != NULL) {
        delete_cfunction(ctype->function);
    }
    free(ctype);
}
