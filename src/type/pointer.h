#ifndef _UMLCC_TYPE_POINTER_H_
#define _UMLCC_TYPE_POINTER_H_


typedef struct _CType CType;

typedef struct _CPointer {
    CType* to_ctype;
} CPointer;

CPointer* new_cpointer(CType* to_ctype);
CPointer* new_socket_cpointer();
CPointer* cpointer_copy(CPointer* cpointer);
CType*    cpointer_next(CPointer* cpointer);
CPointer* cpointer_connect(CPointer* socket, CType* plug);
void      delete_cpointer(CPointer* cpointer);

#endif
