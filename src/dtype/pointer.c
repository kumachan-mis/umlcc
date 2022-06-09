#include "./pointer.h"

#include <stdlib.h>

struct Dtype* dtype_copy(struct Dtype* dtype);
void delete_dtype(struct Dtype* dtype);
int dtype_equals(struct Dtype* dtype, struct Dtype* other);

DPointer* new_dpointer(struct Dtype* to_dtype) {
    DPointer* dpointer = malloc(sizeof(DPointer));
    dpointer->to_dtype = to_dtype;
    return dpointer;
}

DPointer* new_socket_dpointer() {
    return new_dpointer(NULL);
}

DPointer* dpointer_copy(DPointer* dpointer) {
    DPointer* copied_dpointer = malloc(sizeof(DPointer));
    copied_dpointer->to_dtype = NULL;
    if (dpointer->to_dtype != NULL) copied_dpointer->to_dtype = dtype_copy(dpointer->to_dtype);
    return copied_dpointer;
}

struct Dtype* dpointer_next(DPointer* dpointer) {
    return dpointer->to_dtype;
}

DPointer* dpointer_connect(DPointer* socket, struct Dtype* plug) {
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
