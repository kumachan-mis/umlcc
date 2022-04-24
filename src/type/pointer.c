#include "./pointer.h"

#include <stdlib.h>


CType* ctype_copy(CType* ctype);
void   delete_ctype(CType* ctype);


CPointer* new_cpointer(CType* to_type) {
    CPointer* cpointer = malloc(sizeof(CPointer));
    cpointer->to_type = to_type;
    return cpointer;
}

CPointer* new_socket_cpointer() {
    return new_cpointer(NULL);
}

CPointer* cpointer_copy(CPointer* cpointer) {
    CPointer* copied_cpointer = malloc(sizeof(CPointer));
    copied_cpointer->to_type = NULL;
    if (cpointer->to_type != NULL) {
        copied_cpointer->to_type = ctype_copy(cpointer->to_type);
    }
    return copied_cpointer;
}

CType* cpointer_next(CPointer* cpointer) {
    return cpointer->to_type;
}

CPointer* cpointer_connect(CPointer* socket, CType* plug) {
    socket->to_type = plug;
    return socket;
}

void delete_cpointer(CPointer* cpointer) {
    if (cpointer->to_type != NULL) {
        delete_ctype(cpointer->to_type);
    }
    free(cpointer);
}
