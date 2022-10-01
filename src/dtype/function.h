#ifndef UMLCC_TYPE_FUNCTION_H
#define UMLCC_TYPE_FUNCTION_H

#include "../common/type.h"
#include "../vector/vector.h"

typedef struct DFunction {
    Vector* params;
    struct DType* return_dtype;
} DFunction;

typedef struct DParam {
    char* name;
    struct DType* dtype;
} DParam;

extern BaseType t_dparam;

DFunction* new_dfunction(Vector* params, struct DType* return_dtype);
DFunction* new_socket_dfunction(Vector* params);
DFunction* dfunction_copy(DFunction* dfunction);
struct DType* dfunction_next(DFunction* dfunction);
DFunction* dfunction_connect(DFunction* socket, struct DType* plug);
int dfunction_equals(DFunction* dfunction, DFunction* other);
void delete_dfunction(DFunction* dfunction);

DParam* new_dparam(char* name, struct DType* dtype);
DParam* dparam_copy(DParam* dparam);
int dparam_equals(DParam* dparam, DParam* other);
void delete_dparam(DParam* dparam);

#endif
