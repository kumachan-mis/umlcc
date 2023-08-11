#ifndef UMLCC_DTYPE_H_PRIVATE
#define UMLCC_DTYPE_H_PRIVATE
#endif

#include "./pointer.h"

#include <stdlib.h>

DPointer* new_dpointer(DType* to_dtype) {
    DPointer* dpointer = malloc(sizeof(DPointer));
    dpointer->to_dtype = to_dtype;
    return dpointer;
}

DPointer* new_socket_dpointer(void) {
    return new_dpointer(NULL);
}

DPointer* dpointer_copy(DPointer* dpointer) {
    DPointer* copied_dpointer = malloc(sizeof(DPointer));
    copied_dpointer->to_dtype = NULL;
    if (dpointer->to_dtype != NULL) {
        copied_dpointer->to_dtype = dtype_copy(dpointer->to_dtype);
    }
    return copied_dpointer;
}

DType* dpointer_next(DPointer* dpointer) {
    return dpointer->to_dtype;
}

DPointer* dpointer_connect(DPointer* socket, DType* plug) {
    socket->to_dtype = plug;
    return socket;
}

int dpointer_equals(DPointer* dpointer, DPointer* other) {
    return dtype_equals(dpointer->to_dtype, other->to_dtype);
}

void delete_dpointer(DPointer* dpointer) {
    if (dpointer->to_dtype != NULL) {
        delete_dtype(dpointer->to_dtype);
    }
    free(dpointer);
}
