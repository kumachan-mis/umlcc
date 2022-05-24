#include "./pointer.h"

#include <stdlib.h>

struct Dtype* dtype_copy(struct Dtype* dtype);
void delete_dtype(struct Dtype* dtype);

DPointer* new_dpointer(struct Dtype* ptr_to) {
    DPointer* dpointer = malloc(sizeof(DPointer));
    dpointer->ptr_to = ptr_to;
    return dpointer;
}

DPointer* new_socket_dpointer() {
    return new_dpointer(NULL);
}

DPointer* dpointer_copy(DPointer* dpointer) {
    DPointer* copied_dpointer = malloc(sizeof(DPointer));
    copied_dpointer->ptr_to = NULL;
    if (dpointer->ptr_to != NULL) copied_dpointer->ptr_to = dtype_copy(dpointer->ptr_to);
    return copied_dpointer;
}

struct Dtype* dpointer_next(DPointer* dpointer) {
    return dpointer->ptr_to;
}

DPointer* dpointer_connect(DPointer* socket, struct Dtype* plug) {
    socket->ptr_to = plug;
    return socket;
}

void delete_dpointer(DPointer* dpointer) {
    if (dpointer->ptr_to != NULL) delete_dtype(dpointer->ptr_to);
    free(dpointer);
}
