#include "./pointer.h"

#include <stdlib.h>

struct DType* dtype_copy(struct DType* dtype);
void delete_dtype(struct DType* dtype);
int dtype_equals(struct DType* dtype, struct DType* other);

DPointer* new_dpointer(struct DType* to_dtype) {
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
    if (dpointer->to_dtype != NULL) copied_dpointer->to_dtype = dtype_copy(dpointer->to_dtype);
    return copied_dpointer;
}

struct DType* dpointer_next(DPointer* dpointer) {
    return dpointer->to_dtype;
}

DPointer* dpointer_connect(DPointer* socket, struct DType* plug) {
    socket->to_dtype = plug;
    return socket;
}

int dpointer_equals(DPointer* dpointer, DPointer* other) {
    return dtype_equals(dpointer->to_dtype, other->to_dtype);
}

void delete_dpointer(DPointer* dpointer) {
    if (dpointer->to_dtype != NULL) delete_dtype(dpointer->to_dtype);
    free(dpointer);
}
