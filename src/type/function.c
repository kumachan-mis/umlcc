#include "./function.h"
#include "../common/common.h"

#include <stdlib.h>

struct _CType* ctype_copy(struct _CType* ctype);
void delete_ctype(struct _CType* ctype);

CFunction* new_cfunction(Vector* params, struct _CType* return_ctype) {
    CFunction* cfunction = malloc(sizeof(CFunction));
    cfunction->params = params;
    cfunction->return_ctype = return_ctype;
    return cfunction;
}

CFunction* new_socket_cfunction(Vector* params) {
    return new_cfunction(params, NULL);
}

CFunction* cfunction_copy(CFunction* cfunction) {
    CFunction* copied_cfunction = malloc(sizeof(CFunction));

    Vector* copied_params = new_vector();
    int num_args = vector_size(cfunction->params);
    for (int i = 0; i < num_args; i++) {
        CParam* copied_param = cparam_copy(vector_at(cfunction->params, i));
        vector_push(copied_params, copied_param);
    }
    copied_cfunction->params = copied_params;

    copied_cfunction->return_ctype = NULL;
    if (cfunction->return_ctype != NULL) {
        copied_cfunction->return_ctype = ctype_copy(cfunction->return_ctype);
    }

    return copied_cfunction;
}

struct _CType* cfunction_next(CFunction* cfunction) {
    return cfunction->return_ctype;
}

CFunction* cfunction_connect(CFunction* socket, struct _CType* plug) {
    socket->return_ctype = plug;
    return socket;
}

void delete_cfunction(CFunction* cfunction) {
    delete_vector(cfunction->params, (void (*)(void* item))delete_cparam);
    if (cfunction->return_ctype != NULL) { delete_ctype(cfunction->return_ctype); }
    free(cfunction);
}

CParam* new_cparam(char* ident_name, struct _CType* ctype) {
    CParam* cparam = malloc(sizeof(CParam));
    cparam->ident_name = ident_name;
    cparam->ctype = ctype;
    return cparam;
}

CParam* cparam_copy(CParam* cparam) {
    CParam* copied_cparam = malloc(sizeof(CParam));
    copied_cparam->ident_name = string_copy(cparam->ident_name);
    copied_cparam->ctype = ctype_copy(cparam->ctype);
    return copied_cparam;
}

void delete_cparam(CParam* cparam) {
    free(cparam->ident_name);
    delete_ctype(cparam->ctype);
    free(cparam);
}
