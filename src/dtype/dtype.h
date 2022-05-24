#ifndef UMLCC_DTYPE_H
#define UMLCC_DTYPE_H

#include "../common/common.h"
#include "../vector/vector.h"
#include "./function.h"
#include "./pointer.h"

typedef enum DtypeType {
    DTYPE_INT,
    DTYPE_POINTER,
    DTYPE_FUNCUCTION,
} DtypeType;

typedef struct Dtype {
    DtypeType type;
    DPointer* pointer;
    DFunction* function;
} Dtype;

extern BaseType t_dtype;

Dtype* new_integer_dtype();
Dtype* new_pointer_dtype(Dtype* to_dtype);
Dtype* new_socket_pointer_dtype();
Dtype* new_function_dtype(Vector* params, Dtype* return_dtype);
Dtype* new_socket_function_dtype(Vector* params);
Dtype* dtype_copy(Dtype* dtype);
Dtype* dtype_connect(Dtype* socket, Dtype* plug);
int dtype_size(Dtype* dtype);
void delete_dtype(Dtype* dtype);

#endif
