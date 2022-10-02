#ifndef UMLCC_DTYPE_H_PRIVATE
#define UMLCC_DTYPE_H_PRIVATE
#endif

#include "./dtype.h"

#include <stdlib.h>

BaseType t_dtype = {
    .copy_object = (void* (*)(void*))dtype_copy,
    .delete_object = (void (*)(void*))delete_dtype,
};

DType* new_integer_dtype(DTypeType type) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = type;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_pointer_dtype(DType* to_dtype) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_POINTER;
    dtype->dpointer = new_dpointer(to_dtype);
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_array_dtype(DType* of_dtype, int size) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_ARRAY;
    dtype->dpointer = NULL;
    dtype->darray = new_darray(of_dtype, size);
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_named_struct_dtype(char* name) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_STRUCT;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = new_named_dstruct(name);
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_unnamed_struct_dtype(Vector* members) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_STRUCT;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = new_unnamed_dstruct(members);
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_function_dtype(Vector* params, DType* return_dtype) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_FUNCTION;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = new_dfunction(params, return_dtype);
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_decoration_dtype(DType* deco_dtype) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_DECORATION;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = new_ddecoration(deco_dtype);
    return dtype;
}

DType* new_socket_pointer_dtype(void) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_POINTER;
    dtype->dpointer = new_socket_dpointer();
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_socket_array_dtype(int size) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_ARRAY;
    dtype->dpointer = NULL;
    dtype->darray = new_socket_darray(size);
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_socket_function_dtype(Vector* params) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_FUNCTION;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = new_socket_dfunction(params);
    dtype->ddecoration = NULL;
    return dtype;
}

DType* new_socket_decoration_dtype(void) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = DTYPE_DECORATION;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->dfunction = NULL;
    dtype->ddecoration = new_socket_ddecoration();
    return dtype;
}

DType* dtype_copy(DType* dtype) {
    DType* copied_dtype = malloc(sizeof(DType));
    copied_dtype->type = dtype->type;

    copied_dtype->dpointer = NULL;
    if (dtype->dpointer != NULL) copied_dtype->dpointer = dpointer_copy(dtype->dpointer);
    copied_dtype->darray = NULL;
    if (dtype->darray != NULL) copied_dtype->darray = darray_copy(dtype->darray);
    copied_dtype->dstruct = NULL;
    if (dtype->dstruct != NULL) copied_dtype->dstruct = dstruct_copy(dtype->dstruct);
    copied_dtype->dfunction = NULL;
    if (dtype->dfunction != NULL) copied_dtype->dfunction = dfunction_copy(dtype->dfunction);
    copied_dtype->ddecoration = NULL;
    if (dtype->ddecoration != NULL) copied_dtype->ddecoration = ddecoration_copy(dtype->ddecoration);
    return copied_dtype;
}

DType* dtype_connect(DType* socket_dtype, DType* plug_dtype) {
    if (socket_dtype == NULL) return plug_dtype;

    DType* tail = socket_dtype;
    while (1) {
        switch (tail->type) {
            case DTYPE_CHAR:
            case DTYPE_INT:
            case DTYPE_STRUCT:
                return socket_dtype;
            case DTYPE_POINTER: {
                DType* next = dpointer_next(tail->dpointer);
                if (next == NULL) {
                    tail->dpointer = dpointer_connect(tail->dpointer, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_ARRAY: {
                DType* next = darray_next(tail->darray);
                if (next == NULL) {
                    tail->darray = darray_connect(tail->darray, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_FUNCTION: {
                DType* next = dfunction_next(tail->dfunction);
                if (next == NULL) {
                    tail->dfunction = dfunction_connect(tail->dfunction, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            case DTYPE_DECORATION: {
                DType* next = ddecoration_next(tail->ddecoration);
                if (next == NULL) {
                    tail->ddecoration = ddecoration_connect(tail->ddecoration, plug_dtype);
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
            return dpointer_equals(dtype->dpointer, other->dpointer);
        case DTYPE_ARRAY:
            return darray_equals(dtype->darray, other->darray);
        case DTYPE_STRUCT:
            return dstruct_equals(dtype->dstruct, other->dstruct);
        case DTYPE_FUNCTION:
            return dfunction_equals(dtype->dfunction, other->dfunction);
        case DTYPE_DECORATION:
            return ddecoration_equals(dtype->ddecoration, other->ddecoration);
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
        dtype_iscompatible(dtype->dpointer->to_dtype, other->dpointer->to_dtype)) {
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
    return DTYPE_ARRAY <= dtype->type && dtype->type <= DTYPE_STRUCT;
}

int dtype_isobject(DType* dtype) {
    return (DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_ARRAY) ||
           (dtype->type == DTYPE_STRUCT && dtype->dstruct->members != NULL);
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
            return dtype->darray->size * dtype_size(dtype->darray->of_dtype);
        case DTYPE_STRUCT: {
            if (dtype->dstruct->members == NULL) return 0;
            int size = 0, num_members = vector_size(dtype->dstruct->members);
            for (int i = 0; i < num_members; i++) {
                DMember* member = vector_at(dtype->dstruct->members, i);
                size += dtype_size(member->dtype);
            }
            return size;
        }
        default:
            return 0;
    }
}

void delete_dtype(DType* dtype) {
    if (dtype->dpointer != NULL) delete_dpointer(dtype->dpointer);
    if (dtype->darray != NULL) delete_darray(dtype->darray);
    if (dtype->dstruct != NULL) delete_dstruct(dtype->dstruct);
    if (dtype->dfunction != NULL) delete_dfunction(dtype->dfunction);
    if (dtype->ddecoration != NULL) delete_ddecoration(dtype->ddecoration);
    free(dtype);
}
