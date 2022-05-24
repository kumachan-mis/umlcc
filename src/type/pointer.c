#include "./pointer.h"

#include <stdlib.h>

struct CType* ctype_copy(struct CType* ctype);
void delete_ctype(struct CType* ctype);

CPointer* new_cpointer(struct CType* to_ctype) {
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
    if (cpointer->to_ctype != NULL) copied_cpointer->to_ctype = ctype_copy(cpointer->to_ctype);
    return copied_cpointer;
}

struct CType* cpointer_next(CPointer* cpointer) {
    return cpointer->to_ctype;
}

CPointer* cpointer_connect(CPointer* socket, struct CType* plug) {
    socket->to_ctype = plug;
    return socket;
}

void delete_cpointer(CPointer* cpointer) {
    if (cpointer->to_ctype != NULL) delete_ctype(cpointer->to_ctype);
    free(cpointer);
}
