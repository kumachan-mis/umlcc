#ifndef UMLCC_DTYPE_H_PRIVATE
#define UMLCC_DTYPE_H_PRIVATE
#endif

#include "./typedef.h"

#include <stdlib.h>

DTypedef* new_dtypedef(DType* defined_dtype) {
    DTypedef* dtypedef = malloc(sizeof(DTypedef));
    dtypedef->defined_dtype = defined_dtype;
    return dtypedef;
}

DTypedef* new_socket_dtypedef(void) {
    return new_dtypedef(NULL);
}

DTypedef* dtypedef_copy(DTypedef* dtypedef) {
    DTypedef* copied_dtypedef = malloc(sizeof(DTypedef));
    copied_dtypedef->defined_dtype = dtype_copy(dtypedef->defined_dtype);
    return copied_dtypedef;
}

DType* dtypedef_next(DTypedef* dtypedef) {
    return dtypedef->defined_dtype;
}

DTypedef* dtypedef_connect(DTypedef* socket, DType* plug) {
    socket->defined_dtype = plug;
    return socket;
}

int dtypedef_equals(DTypedef* dtypedef, DTypedef* other) {
    return dtype_equals(dtypedef->defined_dtype, other->defined_dtype);
}

void delete_dtypedef(DTypedef* dtypedef) {
    if (dtypedef->defined_dtype != NULL) {
        delete_dtype(dtypedef->defined_dtype);
    }
    free(dtypedef);
}
