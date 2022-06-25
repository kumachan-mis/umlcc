#include "./dtype.h"

#include <stdlib.h>

BaseType t_dtype = {
    .copy_object = (void* (*)(void*))dtype_copy,
    .delete_object = (void (*)(void*))delete_dtype,
};

Dtype* new_integer_dtype(DtypeType type) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = type;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_pointer_dtype(Dtype* to_dtype) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_POINTER;
    dtype->pointer = new_dpointer(to_dtype);
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_array_dtype(Dtype* of_dtype, int size) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_ARRAY;
    dtype->pointer = NULL;
    dtype->array = new_darray(of_dtype, size);
    dtype->function = NULL;
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_function_dtype(Vector* params, Dtype* return_dtype) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_FUNCUCTION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = new_dfunction(params, return_dtype);
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_definition_dtype(Dtype* def_dtype) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_DEFINITION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->definition = new_ddefinition(def_dtype);
    return dtype;
}

Dtype* dtype_copy(Dtype* dtype) {
    Dtype* copied_dtype = malloc(sizeof(Dtype));
    copied_dtype->type = dtype->type;

    copied_dtype->pointer = NULL;
    if (dtype->pointer != NULL) copied_dtype->pointer = dpointer_copy(dtype->pointer);
    copied_dtype->array = NULL;
    if (dtype->array != NULL) copied_dtype->array = darray_copy(dtype->array);
    copied_dtype->function = NULL;
    if (dtype->function != NULL) copied_dtype->function = dfunction_copy(dtype->function);
    copied_dtype->definition = NULL;
    if (dtype->definition != NULL) copied_dtype->definition = ddefinition_copy(dtype->definition);
    return copied_dtype;
}

Dtype* new_socket_pointer_dtype() {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_POINTER;
    dtype->pointer = new_socket_dpointer();
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_socket_array_dtype(int size) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_ARRAY;
    dtype->pointer = NULL;
    dtype->array = new_socket_darray(size);
    dtype->function = NULL;
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_socket_function_dtype(Vector* params) {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_FUNCUCTION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = new_socket_dfunction(params);
    dtype->definition = NULL;
    return dtype;
}

Dtype* new_socket_definition_dtype() {
    Dtype* dtype = malloc(sizeof(Dtype));
    dtype->type = DTYPE_DEFINITION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->definition = new_socket_ddefinition();
    return dtype;
}

Dtype* dtype_connect(Dtype* socket_dtype, Dtype* plug_dtype) {
    if (socket_dtype == NULL) return plug_dtype;

    Dtype* tail = socket_dtype;
    while (1) {
        switch (tail->type) {
            case DTYPE_CHAR:
            case DTYPE_INT:
                return socket_dtype;
            case DTYPE_POINTER: {
                Dtype* next = dpointer_next(tail->pointer);
                if (next == NULL) {
                    tail->pointer = dpointer_connect(tail->pointer, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_ARRAY: {
                Dtype* next = darray_next(tail->array);
                if (next == NULL) {
                    tail->array = darray_connect(tail->array, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_FUNCUCTION: {
                Dtype* next = dfunction_next(tail->function);
                if (next == NULL) {
                    tail->function = dfunction_connect(tail->function, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_DEFINITION: {
                Dtype* next = ddefinition_next(tail->definition);
                if (next == NULL) {
                    tail->definition = ddefinition_connect(tail->definition, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
        }
    }
}

int dtype_equals(Dtype* dtype, Dtype* other) {
    if (dtype->type != other->type) return 0;

    switch (dtype->type) {
        case DTYPE_CHAR:
        case DTYPE_INT:
            return 1;
        case DTYPE_POINTER:
            return dpointer_equals(dtype->pointer, other->pointer);
        case DTYPE_ARRAY:
            return darray_equals(dtype->array, other->array);
        case DTYPE_FUNCUCTION:
            return dfunction_equals(dtype->function, other->function);
        case DTYPE_DEFINITION:
            return ddefinition_equals(dtype->definition, other->definition);
        default:
            return 0;
    }
}

int dtype_isinteger(Dtype* dtype) {
    return DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT;
}

int dtype_isarithmetic(Dtype* dtype) {
    return DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT;
}

int dtype_isscalar(Dtype* dtype) {
    return (DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT) || dtype->type == DTYPE_POINTER;
}

int dtype_isaggregate(Dtype* dtype) {
    return DTYPE_ARRAY <= dtype->type && dtype->type <= DTYPE_ARRAY;
}

int dtype_size(Dtype* dtype) {
    switch (dtype->type) {
        case DTYPE_CHAR:
            return 1;
        case DTYPE_INT:
            return 4;
        case DTYPE_POINTER:
            return 8;
        case DTYPE_ARRAY:
            return dtype->array->size * dtype_size(dtype->array->of_dtype);
        default:
            return 0;
    }
}

void delete_dtype(Dtype* dtype) {
    if (dtype->pointer != NULL) delete_dpointer(dtype->pointer);
    if (dtype->array != NULL) delete_darray(dtype->array);
    if (dtype->function != NULL) delete_dfunction(dtype->function);
    if (dtype->definition != NULL) delete_ddefinition(dtype->definition);
    free(dtype);
}
