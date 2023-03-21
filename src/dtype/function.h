#ifndef UMLCC_DTYPE_FUNCTION_H
#define UMLCC_DTYPE_FUNCTION_H

#include "../common/type.h"
#include "../vector/vector.h"

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;

DType* dtype_copy(DType* dtype);
int dtype_equals(DType* dtype, DType* other);
void delete_dtype(DType* dtype);
#endif

typedef struct DFunction DFunction;
typedef struct DParam DParam;

struct DFunction {
    Vector* params;
    DType* return_dtype;
};

struct DParam {
    char* name;
    DType* dtype;
};

extern BaseType t_dparam;

#ifdef UMLCC_DTYPE_H_PRIVATE
DFunction* new_dfunction(Vector* params, DType* return_dtype);
DFunction* new_socket_dfunction(Vector* params);
DFunction* dfunction_copy(DFunction* dfunction);
DType* dfunction_next(DFunction* dfunction);
DFunction* dfunction_connect(DFunction* socket, DType* plug);
int dfunction_equals(DFunction* dfunction, DFunction* other);
void delete_dfunction(DFunction* dfunction);
#endif

DParam* new_named_dparam(char* name, DType* dtype);
DParam* new_unnamed_dparam(DType* dtype);
DParam* dparam_copy(DParam* dparam);
int dparam_equals(DParam* dparam, DParam* other);
void delete_dparam(DParam* dparam);

#endif
