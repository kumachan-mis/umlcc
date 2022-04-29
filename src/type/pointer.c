#include "./pointer.h"

#include <stdlib.h>


CType* ctype_copy(CType* ctype);
void   delete_ctype(CType* ctype);


CPointer* new_cpointer(CType* to_ctype) {
    CPointer* cpointer = malloc(sizeof(CPointer));
    cpointer->to_ctype = to_ctype;
    return cpointer;
}

CPointer* new_socket_cpointer() {
    return new_cpointer(NULL);
}

CPointer* cpointer_copy(CPointer* cpointer) {
    CPointer* copied_cpointer = malloc(sizeof(CPointer));
    copied_cpointer->to_ctype = NULL;
    if (cpointer->to_ctype != NULL) {
        copied_cpointer->to_ctype = ctype_copy(cpointer->to_ctype);
    }
    return copied_cpointer;
}

CType* cpointer_next(CPointer* cpointer) {
    return cpointer->to_ctype;
}

CPointer* cpointer_connect(CPointer* socket, CType* plug) {
    socket->to_ctype = plug;
    return socket;
}

void delete_cpointer(CPointer* cpointer) {
    if (cpointer->to_ctype != NULL) {
        delete_ctype(cpointer->to_ctype);
    }
    free(cpointer);
}
