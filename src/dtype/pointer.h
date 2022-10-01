#ifndef UMLCC_TYPE_POINTER_H
#define UMLCC_TYPE_POINTER_H

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;
#endif

typedef struct DPointer DPointer;

struct DPointer {
    DType* to_dtype;
};

DPointer* new_dpointer(DType* to_dtype);
DPointer* new_socket_dpointer(void);
DPointer* dpointer_copy(DPointer* dpointer);
DType* dpointer_next(DPointer* dpointer);
DPointer* dpointer_connect(DPointer* socket, DType* plug);
int dpointer_equals(DPointer* dpointer, DPointer* other);
void delete_dpointer(DPointer* dpointer);

#endif
