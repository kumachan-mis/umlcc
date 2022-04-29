#ifndef _UMLCC_TYPE_POINTER_H_
#define _UMLCC_TYPE_POINTER_H_


typedef struct _CPointer {
    struct _CType* to_ctype;
} CPointer;

CPointer*      new_cpointer(struct _CType* to_ctype);
CPointer*      new_socket_cpointer();
CPointer*      cpointer_copy(CPointer* cpointer);
struct _CType* cpointer_next(CPointer* cpointer);
CPointer*      cpointer_connect(CPointer* socket, struct _CType* plug);
void           delete_cpointer(CPointer* cpointer);

#endif
