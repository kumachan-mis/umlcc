#include "./type.h"

#include <stdlib.h>


CType* new_integer_ctype() {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_INT;
    ctype->func_args = NULL;
    ctype->func_return = NULL;
    return ctype;
}

CType* new_function_ctype(Vector* func_args, CType* func_return) {
    CType* ctype = malloc(sizeof(CType));
    ctype->type = CTYPE_FUNC;
    ctype->func_args = func_args;
    ctype->func_return = func_return;
    return ctype;
}

CType* new_function_socket_ctype(Vector* func_args) {
    return new_function_ctype(func_args, NULL);
}

CType* ctype_copy(CType* ctype) {
    CType* copied_ctype = malloc(sizeof(CType));
    copied_ctype->type = ctype->type;

    if (ctype->func_args != NULL) {
        Vector* copied_func_args = new_vector();
        int num_args = vector_size(ctype->func_args);
        for (int i = 0; i < num_args; i++) {
            CType* copied_func_arg = ctype_copy(vector_at(ctype->func_args, i));
            vector_push(copied_func_args, copied_func_arg);
        }
        copied_ctype->func_args = copied_func_args;
    }

    if (ctype->func_return != NULL) {
        copied_ctype->func_return = ctype_copy(ctype->func_return);
    }

    return copied_ctype;
}

CType* ctype_connect(CType* socket, CType* plug) {
    if (socket == NULL) return plug;

    CType* ctype = socket;
    while (1) {
        switch (ctype->type) {
            case CTYPE_FUNC:
                if (ctype->func_return == NULL){
                    ctype->func_return = plug;
                    return socket;
                }
                ctype = ctype->func_return;
                break;
            case CTYPE_INT:
                return socket;
        }
    }
}

int ctype_size(CType* ctype) {
    switch (ctype->type) {
        case CTYPE_INT:
            return 4;
        case CTYPE_FUNC:
            return 0;
        default:
            return -1;
    }
}

void delete_ctype(CType* ctype) {
    if (ctype->func_args != NULL) {
        delete_vector(ctype->func_args, (void (*)(void* item))delete_ctype);
    }
    if (ctype->func_return != NULL) {
        delete_ctype(ctype->func_return);
    }
    free(ctype);
}
