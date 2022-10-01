#ifndef UMLCC_TYPE_FUNCTION_H
#define UMLCC_TYPE_FUNCTION_H

#include "../common/type.h"
#include "../vector/vector.h"

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;
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

DFunction* new_dfunction(Vector* params, DType* return_dtype);
DFunction* new_socket_dfunction(Vector* params);
DFunction* dfunction_copy(DFunction* dfunction);
DType* dfunction_next(DFunction* dfunction);
DFunction* dfunction_connect(DFunction* socket, DType* plug);
int dfunction_equals(DFunction* dfunction, DFunction* other);
void delete_dfunction(DFunction* dfunction);

DParam* new_dparam(char* name, DType* dtype);
DParam* dparam_copy(DParam* dparam);
int dparam_equals(DParam* dparam, DParam* other);
void delete_dparam(DParam* dparam);

#endif
