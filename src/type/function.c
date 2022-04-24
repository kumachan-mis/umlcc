#include "./function.h"

#include <stdlib.h>


CType* ctype_copy(CType* ctype);
void   delete_ctype(CType* ctype);


CFunction* new_cfunction(Vector* arg_types, CType* return_type) {
    CFunction* cfunction = malloc(sizeof(CFunction));
    cfunction->arg_types = arg_types;
    cfunction->return_type = return_type;
    return cfunction;
}

CFunction* new_socket_cfunction(Vector* arg_types) {
    return new_cfunction(arg_types, NULL);
}

CFunction* cfunction_copy(CFunction* cfunction) {
    CFunction* copied_cfunction = malloc(sizeof(CFunction));

    Vector* copied_arg_types = new_vector();
    int num_args = vector_size(cfunction->arg_types);
    for (int i = 0; i < num_args; i++) {
        CType* copied_arg_type = ctype_copy(vector_at(cfunction->arg_types, i));
        vector_push(copied_arg_types, copied_arg_type);
    }
    copied_cfunction->arg_types = copied_arg_types;

    copied_cfunction->return_type = NULL;
    if (cfunction->return_type != NULL) {
        copied_cfunction->return_type = ctype_copy(cfunction->return_type);
    }

    return copied_cfunction;
}

CType* cfunction_next(CFunction* cfunction) {
    return cfunction->return_type;
}

CFunction* cfunction_connect(CFunction* socket, CType* plug) {
    socket->return_type = plug;
    return socket;
}

void delete_cfunction(CFunction* cfunction) {
    delete_vector(cfunction->arg_types, (void (*)(void* item))delete_ctype);
    if (cfunction->return_type != NULL) {
        delete_ctype(cfunction->return_type);
    }
    free(cfunction);
}
