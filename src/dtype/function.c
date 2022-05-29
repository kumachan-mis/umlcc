#include "./function.h"
#include "../common/type.h"

#include <stdlib.h>

BaseType t_dparam = {
    .copy_object = (void* (*)(void*))dparam_copy,
    .delete_object = (void (*)(void*))delete_dparam,
};

struct Dtype* dtype_copy(struct Dtype* dtype);
void delete_dtype(struct Dtype* dtype);

DFunction* new_dfunction(Vector* params, struct Dtype* return_dtype) {
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

    Vector* copied_params = new_vector(&t_dparam);
    int num_args = vector_size(dfunction->params);
    for (int i = 0; i < num_args; i++) {
        DParam* copied_param = dparam_copy(vector_at(dfunction->params, i));
        vector_push(copied_params, copied_param);
    }
    copied_dfunction->params = copied_params;

    copied_dfunction->return_dtype = NULL;
    if (dfunction->return_dtype != NULL) {
        copied_dfunction->return_dtype = dtype_copy(dfunction->return_dtype);
    }

    return copied_dfunction;
}

struct Dtype* dfunction_next(DFunction* dfunction) {
    return dfunction->return_dtype;
}

DFunction* dfunction_connect(DFunction* socket, struct Dtype* plug) {
    socket->return_dtype = plug;
    return socket;
}

void delete_dfunction(DFunction* dfunction) {
    delete_vector(dfunction->params);
    if (dfunction->return_dtype != NULL) delete_dtype(dfunction->return_dtype);
    free(dfunction);
}

DParam* new_dparam(char* ident_name, struct Dtype* dtype) {
    DParam* dparam = malloc(sizeof(DParam));
    dparam->ident_name = ident_name;
    dparam->dtype = dtype;
    return dparam;
}

DParam* dparam_copy(DParam* dparam) {
    DParam* copied_dparam = malloc(sizeof(DParam));
    copied_dparam->ident_name = new_string(dparam->ident_name);
    copied_dparam->dtype = dtype_copy(dparam->dtype);
    return copied_dparam;
}

void delete_dparam(DParam* dparam) {
    free(dparam->ident_name);
    delete_dtype(dparam->dtype);
    free(dparam);
}
