#define UMLCC_DTYPE_H_PRIVATE

#include "./dtype.h"

#include <stdlib.h>

BaseType t_dtype = {
    .copy_object = (void* (*)(void*))dtype_copy,
    .delete_object = (void (*)(void*))delete_dtype,
};

DType* new_integer_dtype(DTypeType type) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = type;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->decoration = NULL;
    return dtype;
}

DType* new_pointer_dtype(DType* to_dtype) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_POINTER;
    dtype->pointer = new_dpointer(to_dtype);
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->decoration = NULL;
    return dtype;
}

DType* new_array_dtype(DType* of_dtype, int size) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_ARRAY;
    dtype->pointer = NULL;
    dtype->array = new_darray(of_dtype, size);
    dtype->function = NULL;
    dtype->decoration = NULL;
    return dtype;
}

DType* new_function_dtype(Vector* params, DType* return_dtype) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_FUNCTION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = new_dfunction(params, return_dtype);
    dtype->decoration = NULL;
    return dtype;
}

DType* new_decoration_dtype(DType* deco_dtype) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_DECORATION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->decoration = new_ddecoration(deco_dtype);
    return dtype;
}

DType* dtype_copy(DType* dtype) {
    DType* copied_dtype = malloc(sizeof(DType));
    copied_dtype->type = dtype->type;

    copied_dtype->pointer = NULL;
    if (dtype->pointer != NULL) copied_dtype->pointer = dpointer_copy(dtype->pointer);
    copied_dtype->array = NULL;
    if (dtype->array != NULL) copied_dtype->array = darray_copy(dtype->array);
    copied_dtype->function = NULL;
    if (dtype->function != NULL) copied_dtype->function = dfunction_copy(dtype->function);
    copied_dtype->decoration = NULL;
    if (dtype->decoration != NULL) copied_dtype->decoration = ddecoration_copy(dtype->decoration);
    return copied_dtype;
}

DType* new_socket_pointer_dtype(void) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_POINTER;
    dtype->pointer = new_socket_dpointer();
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->decoration = NULL;
    return dtype;
}

DType* new_socket_array_dtype(int size) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_ARRAY;
    dtype->pointer = NULL;
    dtype->array = new_socket_darray(size);
    dtype->function = NULL;
    dtype->decoration = NULL;
    return dtype;
}

DType* new_socket_function_dtype(Vector* params) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_FUNCTION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = new_socket_dfunction(params);
    dtype->decoration = NULL;
    return dtype;
}

DType* new_socket_decoration_dtype(void) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_DECORATION;
    dtype->pointer = NULL;
    dtype->array = NULL;
    dtype->function = NULL;
    dtype->decoration = new_socket_ddecoration();
    return dtype;
}

DType* dtype_connect(DType* socket_dtype, DType* plug_dtype) {
    if (socket_dtype == NULL) return plug_dtype;

    DType* tail = socket_dtype;
    while (1) {
        switch (tail->type) {
            case DTYPE_CHAR:
            case DTYPE_INT:
                return socket_dtype;
            case DTYPE_POINTER: {
                DType* next = dpointer_next(tail->pointer);
                if (next == NULL) {
                    tail->pointer = dpointer_connect(tail->pointer, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_ARRAY: {
                DType* next = darray_next(tail->array);
                if (next == NULL) {
                    tail->array = darray_connect(tail->array, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_FUNCTION: {
                DType* next = dfunction_next(tail->function);
                if (next == NULL) {
                    tail->function = dfunction_connect(tail->function, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_DECORATION: {
                DType* next = ddecoration_next(tail->decoration);
                if (next == NULL) {
                    tail->decoration = ddecoration_connect(tail->decoration, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
        }
    }
}

int dtype_equals(DType* dtype, DType* other) {
    if (dtype->type != other->type) return 0;

    switch (dtype->type) {
        case DTYPE_CHAR:
        case DTYPE_INT:
            return 1;
        case DTYPE_POINTER:
            return dpointer_equals(dtype->pointer, other->pointer);
        case DTYPE_ARRAY:
            return darray_equals(dtype->array, other->array);
        case DTYPE_FUNCTION:
            return dfunction_equals(dtype->function, other->function);
        case DTYPE_DECORATION:
            return ddecoration_equals(dtype->decoration, other->decoration);
        default:
            return 0;
    }
}

int dtype_iscompatible(DType* dtype, DType* other) {
    if (dtype_equals(dtype, other)) return 1;

    // TODO: more rules may be added
    return 0;
}

int dtype_isassignable(DType* dtype, DType* other) {
    if (dtype_isarithmetic(dtype) && dtype_isarithmetic(other)) return 1;

    if (dtype->type == DTYPE_POINTER && other->type == DTYPE_POINTER &&
        dtype_iscompatible(dtype->pointer->to_dtype, other->pointer->to_dtype)) {
        return 1;
    }

    // TODO: more rules may be added
    return 0;
}

int dtype_isinteger(DType* dtype) {
    return DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT;
}

int dtype_isarithmetic(DType* dtype) {
    return DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT;
}

int dtype_isscalar(DType* dtype) {
    return DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_POINTER;
}

int dtype_isaggregate(DType* dtype) {
    return DTYPE_ARRAY <= dtype->type && dtype->type <= DTYPE_ARRAY;
}

int dtype_isobject(DType* dtype) {
    return DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_ARRAY;
}

int dtype_size(DType* dtype) {
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

void delete_dtype(DType* dtype) {
    if (dtype->pointer != NULL) delete_dpointer(dtype->pointer);
    if (dtype->array != NULL) delete_darray(dtype->array);
    if (dtype->function != NULL) delete_dfunction(dtype->function);
    if (dtype->decoration != NULL) delete_ddecoration(dtype->decoration);
    free(dtype);
}
