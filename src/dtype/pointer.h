#ifndef UMLCC_TYPE_POINTER_H
#define UMLCC_TYPE_POINTER_H

typedef struct DPointer {
    struct Dtype* to_dtype;
} DPointer;

DPointer* new_dpointer(struct Dtype* to_dtype);
DPointer* new_socket_dpointer();
DPointer* dpointer_copy(DPointer* dpointer);
struct Dtype* dpointer_next(DPointer* dpointer);
DPointer* dpointer_connect(DPointer* socket, struct Dtype* plug);
int dpointer_equals(DPointer* dpointer, DPointer* other);
void delete_dpointer(DPointer* dpointer);

#endif
