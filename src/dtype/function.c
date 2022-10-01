#include "./function.h"
#include "../common/type.h"

#include <stdlib.h>

BaseType t_dparam = {
    .copy_object = (void* (*)(void*))dparam_copy,
    .delete_object = (void (*)(void*))delete_dparam,
};

struct DType* dtype_copy(struct DType* dtype);
void delete_dtype(struct DType* dtype);
int dtype_equals(struct DType* dtype, struct DType* other);

DFunction* new_dfunction(Vector* params, struct DType* return_dtype) {
    DFunction* dfunction = malloc(sizeof(DFunction));
    dfunction->params = params;
    dfunction->return_dtype = return_dtype;
    return dfunction;
}

DFunction* new_socket_dfunction(Vector* params) {
    return new_dfunction(params, NULL);
}

DFunction* dfunction_copy(DFunction* dfunction) {
    DFunction* copied_dfunction = malloc(sizeof(DFunction));
    copied_dfunction->params = vector_copy(dfunction->params);
    copied_dfunction->return_dtype = NULL;
    if (dfunction->return_dtype != NULL) copied_dfunction->return_dtype = dtype_copy(dfunction->return_dtype);
    return copied_dfunction;
}

struct DType* dfunction_next(DFunction* dfunction) {
    return dfunction->return_dtype;
}

DFunction* dfunction_connect(DFunction* socket, struct DType* plug) {
    socket->return_dtype = plug;
    return socket;
}

int dfunction_equals(DFunction* dfunction, DFunction* other) {
    if (!dtype_equals(dfunction->return_dtype, other->return_dtype)) return 0;
    if (vector_size(dfunction->params) != vector_size(other->params)) return 0;

    int num_params = vector_size(dfunction->params);
    for (int i = 0; i < num_params; i++) {
        DParam* dfunction_dparam = vector_at(dfunction->params, i);
        DParam* other_dparam = vector_at(other->params, i);
        if (!dparam_equals(dfunction_dparam, other_dparam)) return 0;
    }
    return 1;
}

void delete_dfunction(DFunction* dfunction) {
    delete_vector(dfunction->params);
    if (dfunction->return_dtype != NULL) delete_dtype(dfunction->return_dtype);
    free(dfunction);
}

DParam* new_dparam(char* name, struct DType* dtype) {
    DParam* dparam = malloc(sizeof(DParam));
    dparam->name = name;
    dparam->dtype = dtype;
    return dparam;
}

DParam* dparam_copy(DParam* dparam) {
    DParam* copied_dparam = malloc(sizeof(DParam));
    copied_dparam->name = new_string(dparam->name);
    copied_dparam->dtype = dtype_copy(dparam->dtype);
    return copied_dparam;
}

int dparam_equals(DParam* dparam, DParam* other) {
    return dtype_equals(dparam->dtype, other->dtype);
}

void delete_dparam(DParam* dparam) {
    free(dparam->name);
    delete_dtype(dparam->dtype);
    free(dparam);
}
