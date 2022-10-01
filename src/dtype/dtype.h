#ifndef UMLCC_DTYPE_H
#define UMLCC_DTYPE_H

#include "../common/type.h"
#include "../vector/vector.h"
#include "./array.h"
#include "./decoration.h"
#include "./function.h"
#include "./pointer.h"

typedef enum DTypeType {
    DTYPE_CHAR,
    DTYPE_INT,
    DTYPE_POINTER,
    DTYPE_ARRAY,
    DTYPE_FUNCTION,
    DTYPE_DECORATION,
} DTypeType;

typedef struct DType {
    DTypeType type;
    DPointer* pointer;
    DArray* array;
    DFunction* function;
    DDecoration* decoration;
} DType;

extern BaseType t_dtype;

DType* new_integer_dtype(DTypeType type);
DType* new_pointer_dtype(DType* to_dtype);
DType* new_array_dtype(DType* of_dtype, int size);
DType* new_function_dtype(Vector* params, DType* return_dtype);
DType* new_decoration_dtype(DType* deco_dtype);
DType* dtype_copy(DType* dtype);
DType* new_socket_pointer_dtype(void);
DType* new_socket_array_dtype(int size);
DType* new_socket_function_dtype(Vector* params);
DType* new_socket_decoration_dtype(void);
DType* dtype_connect(DType* socket_dtype, DType* plug_dtype);
int dtype_equals(DType* dtype, DType* other);
int dtype_iscompatible(DType* dtype, DType* other);
int dtype_isassignable(DType* dtype, DType* other);
int dtype_isinteger(DType* dtype);
int dtype_isarithmetic(DType* dtype);
int dtype_isscalar(DType* dtype);
int dtype_isaggregate(DType* dtype);
int dtype_isobject(DType* dtype);
int dtype_size(DType* dtype);
void delete_dtype(DType* dtype);

#endif
