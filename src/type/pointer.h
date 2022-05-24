#ifndef UMLCC_TYPE_POINTER_H
#define UMLCC_TYPE_POINTER_H

typedef struct CPointer {
    struct CType* to_ctype;
} CPointer;

CPointer* new_cpointer(struct CType* to_ctype);
CPointer* new_socket_cpointer();
CPointer* cpointer_copy(CPointer* cpointer);
struct CType* cpointer_next(CPointer* cpointer);
CPointer* cpointer_connect(CPointer* socket, struct CType* plug);
void delete_cpointer(CPointer* cpointer);

#endif
