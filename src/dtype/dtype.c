#include "./dtype.h"

#include <stdlib.h>

BaseType t_dtype = {
    .copy_object = dtype_copy,
    .delete_object = delete_dtype,
};

Dtype* new_integer_dtype() {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_INT;
    dtype->pointer = NULL;
    dtype->function = NULL;
    return dtype;
}

Dtype* new_pointer_dtype(Dtype* ptr_to) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_POINTER;
    dtype->pointer = new_dpointer(ptr_to);
    dtype->function = NULL;
    return dtype;
}

Dtype* new_socket_pointer_dtype() {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_POINTER;
    dtype->pointer = new_socket_dpointer();
    dtype->function = NULL;
    return dtype;
}

Dtype* new_function_dtype(Vector* params, Dtype* func_return) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_FUNCUCTION;
    dtype->pointer = NULL;
    dtype->function = new_dfunction(params, func_return);
    return dtype;
}

Dtype* new_socket_function_dtype(Vector* params) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_FUNCUCTION;
    dtype->pointer = NULL;
    dtype->function = new_socket_dfunction(params);
    return dtype;
}

Dtype* dtype_copy(Dtype* dtype) {
    Dtype* copied_dtype = malloc(sizeof(Dtype));
    copied_dtype->type = dtype->type;

    copied_dtype->pointer = NULL;
    if (dtype->pointer != NULL) copied_dtype->pointer = dpointer_copy(dtype->pointer);
    copied_dtype->function = NULL;
    if (dtype->function != NULL) copied_dtype->function = dfunction_copy(dtype->function);
    return copied_dtype;
}

Dtype* dtype_connect(Dtype* socket, Dtype* plug) {
    if (socket == NULL) return plug;

    Dtype* fragment = socket;
    while (1) {
        switch (fragment->type) {
            case DTYPE_INT:
                return socket;
            case DTYPE_POINTER: {
                Dtype* next = dpointer_next(fragment->pointer);
                if (next == NULL) {
                    fragment->pointer = dpointer_connect(fragment->pointer, plug);
                    return socket;
                }
                fragment = next;
                break;
            }
            case DTYPE_FUNCUCTION: {
                Dtype* next = dfunction_next(fragment->function);
                if (next == NULL) {
                    fragment->function = dfunction_connect(fragment->function, plug);
                    return socket;
                }
                fragment = next;
                break;
            }
        }
    }
}

int dtype_size(Dtype* dtype) {
    switch (dtype->type) {
        case DTYPE_INT:
            return 4;
        case DTYPE_POINTER:
            return 8;
        case DTYPE_FUNCUCTION:
            return 0;
        default:
            return -1;
    }
}

void delete_dtype(Dtype* dtype) {
    if (dtype->pointer != NULL) delete_dpointer(dtype->pointer);
    if (dtype->function != NULL) delete_dfunction(dtype->function);
    free(dtype);
}
