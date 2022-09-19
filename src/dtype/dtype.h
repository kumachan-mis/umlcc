#ifndef UMLCC_DTYPE_H
#define UMLCC_DTYPE_H

#include "../common/type.h"
#include "../vector/vector.h"
#include "./array.h"
#include "./decoration.h"
#include "./function.h"
#include "./pointer.h"

typedef enum DtypeType {
    DTYPE_CHAR,
    DTYPE_INT,
    DTYPE_POINTER,
    DTYPE_ARRAY,
    DTYPE_FUNCTION,
    DTYPE_DECORATION,
} DtypeType;

typedef struct Dtype {
    DtypeType type;
    DPointer* pointer;
    DArray* array;
    DFunction* function;
    DDecoration* decoration;
} Dtype;

extern BaseType t_dtype;

extern char* dtype_types[];

Dtype* new_integer_dtype(DtypeType type);
Dtype* new_pointer_dtype(Dtype* to_dtype);
Dtype* new_array_dtype(Dtype* of_dtype, int size);
Dtype* new_function_dtype(Vector* params, Dtype* return_dtype);
Dtype* new_decoration_dtype(Dtype* deco_dtype);
Dtype* dtype_copy(Dtype* dtype);
Dtype* new_socket_pointer_dtype();
Dtype* new_socket_array_dtype(int size);
Dtype* new_socket_function_dtype(Vector* params);
Dtype* new_socket_decoration_dtype();
Dtype* dtype_connect(Dtype* socket_dtype, Dtype* plug_dtype);
int dtype_equals(Dtype* dtype, Dtype* other);
int dtype_iscompatible(Dtype* dtype, Dtype* other);
int dtype_isassignable(Dtype* dtype, Dtype* other);
int dtype_isinteger(Dtype* dtype);
int dtype_isarithmetic(Dtype* dtype);
int dtype_isscalar(Dtype* dtype);
int dtype_isaggregate(Dtype* dtype);
int dtype_isobject(Dtype* dtype);
int dtype_size(Dtype* dtype);
void delete_dtype(Dtype* dtype);

#endif
