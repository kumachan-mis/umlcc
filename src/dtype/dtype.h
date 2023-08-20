#ifndef UMLCC_DTYPE_H
#define UMLCC_DTYPE_H

#include "../common/type.h"
#include "../vector/vector.h"

typedef struct DType DType;

#include "./array.h"
#include "./enum.h"
#include "./function.h"
#include "./pointer.h"
#include "./struct.h"
#include "./typedef.h"

typedef enum DTypeType {
    DTYPE_VOID,
    DTYPE_CHAR,
    DTYPE_INT,
    DTYPE_UNSIGNED_INT,
    DTYPE_LONG,
    DTYPE_UNSIGNED_LONG,
    DTYPE_LONGLONG,
    DTYPE_UNSIGNED_LONGLONG,
    DTYPE_POINTER,
    DTYPE_ARRAY,
    DTYPE_STRUCT,
    DTYPE_ENUM,
    DTYPE_FUNCTION,
    DTYPE_TYPEDEF,
} DTypeType;

struct DType {
    DTypeType type;
    DPointer* dpointer;
    DArray* darray;
    DStruct* dstruct;
    DEnum* denum;
    DFunction* dfunction;
    DTypedef* dtypedef;
};

extern BaseType t_dtype;

DType* new_void_dtype(void);
DType* new_integer_dtype(DTypeType type);
DType* new_pointer_dtype(DType* to_dtype);
DType* new_array_dtype(DType* of_dtype, int size);
DType* new_named_struct_dtype(char* name, int nbytes, int alignment);
DType* new_unnamed_struct_dtype(Vector* members);
DType* new_named_enum_dtype(char* name);
DType* new_unnamed_enum_dtype(Vector* members);
DType* new_function_dtype(Vector* params, DType* return_dtype);
DType* new_typedef_dtype(DType* defined_dtype);
DType* new_socket_pointer_dtype(void);
DType* new_socket_array_dtype(int size);
DType* new_socket_function_dtype(Vector* params);
DType* new_socket_typedef_dtype(void);
DType* dtype_copy(DType* dtype);
DType* dtype_connect(DType* socket_dtype, DType* plug_dtype);
int dtype_scalar_rank(DType* dtype);
DType* dtype_aggregate_at(DType* dtype, int index);
int dtype_aggregate_size(DType* dtype);
int dtype_equals(DType* dtype, DType* other);
int dtype_iscompatible(DType* dtype, DType* other);
int dtype_isassignable(DType* dtype, DType* other);
int dtype_isinteger(DType* dtype);
int dtype_issignedinteger(DType* dtype);
int dtype_isunsignedinteger(DType* dtype);
int dtype_isarithmetic(DType* dtype);
int dtype_isscalar(DType* dtype);
int dtype_isaggregate(DType* dtype);
int dtype_isobject(DType* dtype);
int dtype_isincomplete(DType* dtype);
int dtype_alignment(DType* dtype);
int dtype_nbytes(DType* dtype);
void delete_dtype(DType* dtype);

#endif
