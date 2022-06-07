#ifndef UMLCC_DTYPE_H
#define UMLCC_DTYPE_H

#include "../common/type.h"
#include "../vector/vector.h"
#include "./array.h"
#include "./function.h"
#include "./pointer.h"

typedef enum DtypeType {
    DTYPE_INT,
    DTYPE_POINTER,
    DTYPE_ARRAY,
    DTYPE_FUNCUCTION,
} DtypeType;

typedef struct Dtype {
    DtypeType type;
    DPointer* pointer;
    DArray* array;
    DFunction* function;
} Dtype;

extern BaseType t_dtype;

Dtype* new_integer_dtype();
Dtype* new_pointer_dtype(Dtype* to_dtype);
Dtype* new_array_dtype(Dtype* of_dtype, int size);
Dtype* new_function_dtype(Vector* params, Dtype* return_dtype);
Dtype* dtype_copy(Dtype* dtype);
Dtype* new_socket_pointer_dtype();
Dtype* new_socket_array_dtype(int size);
Dtype* new_socket_function_dtype(Vector* params);
Dtype* dtype_connect(Dtype* socket_dtype, Dtype* plug_dtype);
int dtype_isarithmetic(Dtype* dtype);
int dtype_isscalar(Dtype* dtype);
int dtype_isaggregate(Dtype* dtype);
int dtype_size(Dtype* dtype);
void delete_dtype(Dtype* dtype);

#endif
