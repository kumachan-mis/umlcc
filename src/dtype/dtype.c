#ifndef UMLCC_DTYPE_H_PRIVATE
#define UMLCC_DTYPE_H_PRIVATE
#endif

#include "./dtype.h"

#include <stdlib.h>

BaseType t_dtype = {
    .copy_object = (void* (*)(void*))dtype_copy,
    .delete_object = (void (*)(void*))delete_dtype,
};

DType* new_base_dtype(DTypeType type);

DType* new_void_dtype(void) {
    DType* dtype = new_base_dtype(DTYPE_VOID);
    return dtype;
}

DType* new_integer_dtype(DTypeType type) {
    DType* dtype = new_base_dtype(type);
    return dtype;
}

DType* new_pointer_dtype(DType* to_dtype) {
    DType* dtype = new_base_dtype(DTYPE_POINTER);
    dtype->dpointer = new_dpointer(to_dtype);
    return dtype;
}

DType* new_array_dtype(DType* of_dtype, int size) {
    DType* dtype = new_base_dtype(DTYPE_ARRAY);
    dtype->darray = new_darray(of_dtype, size);
    return dtype;
}

DType* new_named_struct_dtype(char* name, int nbytes, int alignment) {
    DType* dtype = new_base_dtype(DTYPE_STRUCT);
    dtype->dstruct = new_named_dstruct(name, nbytes, alignment);
    return dtype;
}

DType* new_unnamed_struct_dtype(Vector* members) {
    DType* dtype = new_base_dtype(DTYPE_STRUCT);
    dtype->dstruct = new_unnamed_dstruct(members);
    return dtype;
}

DType* new_named_enum_dtype(char* name) {
    DType* dtype = new_base_dtype(DTYPE_ENUM);
    dtype->denum = new_named_denum(name);
    return dtype;
}

DType* new_unnamed_enum_dtype(Vector* members) {
    DType* dtype = new_base_dtype(DTYPE_ENUM);
    dtype->denum = new_unnamed_denum(members);
    return dtype;
}

DType* new_function_dtype(Vector* params, DType* return_dtype) {
    DType* dtype = new_base_dtype(DTYPE_FUNCTION);
    dtype->dfunction = new_dfunction(params, return_dtype);
    return dtype;
}

DType* new_typedef_dtype(DType* defined_dtype) {
    DType* dtype = new_base_dtype(DTYPE_TYPEDEF);
    dtype->dtypedef = new_dtypedef(defined_dtype);
    return dtype;
}

DType* new_socket_pointer_dtype(void) {
    DType* dtype = new_base_dtype(DTYPE_POINTER);
    dtype->dpointer = new_socket_dpointer();
    return dtype;
}

DType* new_socket_array_dtype(int size) {
    DType* dtype = new_base_dtype(DTYPE_ARRAY);
    dtype->darray = new_socket_darray(size);
    return dtype;
}

DType* new_socket_function_dtype(Vector* params) {
    DType* dtype = new_base_dtype(DTYPE_FUNCTION);
    dtype->dfunction = new_socket_dfunction(params);
    return dtype;
}

DType* new_socket_typedef_dtype(void) {
    DType* dtype = new_base_dtype(DTYPE_TYPEDEF);
    dtype->dtypedef = new_socket_dtypedef();
    return dtype;
}

DType* new_base_dtype(DTypeType type) {
    DType* dtype = malloc(sizeof(DType));
    dtype->type = type;
    dtype->dpointer = NULL;
    dtype->darray = NULL;
    dtype->dstruct = NULL;
    dtype->denum = NULL;
    dtype->dfunction = NULL;
    dtype->dtypedef = NULL;
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
    copied_dtype->denum = NULL;
    if (dtype->denum != NULL) copied_dtype->denum = denum_copy(dtype->denum);
    copied_dtype->dfunction = NULL;
    if (dtype->dfunction != NULL) copied_dtype->dfunction = dfunction_copy(dtype->dfunction);
    copied_dtype->dtypedef = NULL;
    if (dtype->dtypedef != NULL) copied_dtype->dtypedef = dtypedef_copy(dtype->dtypedef);
    return copied_dtype;
}

DType* dtype_connect(DType* socket_dtype, DType* plug_dtype) {
    if (socket_dtype == NULL) return plug_dtype;

    DType* tail = socket_dtype;
    while (1) {
        switch (tail->type) {
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
            case DTYPE_TYPEDEF: {
                DType* next = dtypedef_next(tail->dtypedef);
                if (next == NULL) {
                    tail->dtypedef = dtypedef_connect(tail->dtypedef, plug_dtype);
                    return socket_dtype;
                }
                tail = next;
                break;
            }
            default:
                return socket_dtype;
        }
    }
}

DType* dtype_aggregate_at(DType* dtype, int index) {
    switch (dtype->type) {
        case DTYPE_ARRAY:
            return darray_at(dtype->darray, index);
        case DTYPE_STRUCT:
            return dstruct_at(dtype->dstruct, index);
        default:
            return NULL;
    }
}

int dtype_aggregate_size(DType* dtype) {
    switch (dtype->type) {
        case DTYPE_ARRAY:
            return darray_size(dtype->darray);
        case DTYPE_STRUCT:
            return dstruct_size(dtype->dstruct);
        default:
            return 0;
    }
}

int dtype_equals(DType* dtype, DType* other) {
    if (dtype->type != other->type) return 0;

    switch (dtype->type) {
        case DTYPE_POINTER:
            return dpointer_equals(dtype->dpointer, other->dpointer);
        case DTYPE_ARRAY:
            return darray_equals(dtype->darray, other->darray);
        case DTYPE_STRUCT:
            return dstruct_equals(dtype->dstruct, other->dstruct);
        case DTYPE_ENUM:
            return denum_equals(dtype->denum, other->denum);
        case DTYPE_FUNCTION:
            return dfunction_equals(dtype->dfunction, other->dfunction);
        case DTYPE_TYPEDEF:
            return dtypedef_equals(dtype->dtypedef, other->dtypedef);
        default:
            return 1;
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
    return (DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT) || dtype->type == DTYPE_ENUM;
}

int dtype_isarithmetic(DType* dtype) {
    return (DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_INT) || dtype->type == DTYPE_ENUM;
}

int dtype_isscalar(DType* dtype) {
    return (DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_POINTER) || dtype->type == DTYPE_ENUM;
}

int dtype_isaggregate(DType* dtype) {
    return DTYPE_ARRAY <= dtype->type && dtype->type <= DTYPE_STRUCT;
}

int dtype_isobject(DType* dtype) {
    return (DTYPE_CHAR <= dtype->type && dtype->type <= DTYPE_ARRAY) ||
           (dtype->type == DTYPE_STRUCT && dtype->dstruct->nbytes > 0) || dtype->type == DTYPE_ENUM;
}

int dtype_isincomplete(DType* dtype) {
    return dtype->type == DTYPE_VOID || (dtype->type == DTYPE_STRUCT && dtype->dstruct->nbytes <= 0);
}

int dtype_alignment(DType* dtype) {
    switch (dtype->type) {
        case DTYPE_CHAR:
            return 1;
        case DTYPE_INT:
            return 4;
        case DTYPE_POINTER:
            return 8;
        case DTYPE_ARRAY:
            return dtype_alignment(dtype->darray->of_dtype);
        case DTYPE_STRUCT:
            return dtype->dstruct->alignment;
        case DTYPE_ENUM:
            return 4;
        default:
            return 0;
    }
}

int dtype_nbytes(DType* dtype) {
    switch (dtype->type) {
        case DTYPE_CHAR:
            return 1;
        case DTYPE_INT:
            return 4;
        case DTYPE_POINTER:
            return 8;
        case DTYPE_ARRAY:
            return dtype->darray->size * dtype_nbytes(dtype->darray->of_dtype);
        case DTYPE_STRUCT:
            return dtype->dstruct->nbytes;
        case DTYPE_ENUM:
            return 4;
        default:
            return 0;
    }
}

void delete_dtype(DType* dtype) {
    if (dtype->dpointer != NULL) delete_dpointer(dtype->dpointer);
    if (dtype->darray != NULL) delete_darray(dtype->darray);
    if (dtype->dstruct != NULL) delete_dstruct(dtype->dstruct);
    if (dtype->denum != NULL) delete_denum(dtype->denum);
    if (dtype->dfunction != NULL) delete_dfunction(dtype->dfunction);
    if (dtype->dtypedef != NULL) delete_dtypedef(dtype->dtypedef);
    free(dtype);
}
