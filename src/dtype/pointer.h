#ifndef UMLCC_TYPE_POINTER_H
#define UMLCC_TYPE_POINTER_H

typedef struct DPointer {
    struct DType* to_dtype;
} DPointer;

DPointer* new_dpointer(struct DType* to_dtype);
DPointer* new_socket_dpointer(void);
DPointer* dpointer_copy(DPointer* dpointer);
struct DType* dpointer_next(DPointer* dpointer);
DPointer* dpointer_connect(DPointer* socket, struct DType* plug);
int dpointer_equals(DPointer* dpointer, DPointer* other);
void delete_dpointer(DPointer* dpointer);

#endif
