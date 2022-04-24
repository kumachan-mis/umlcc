#ifndef _UMLCC_TYPE_H_
#define _UMLCC_TYPE_H_

#include "./pointer.h"
#include "./function.h"
#include "../vector/vector.h"

typedef enum _CTypeType {
   CTYPE_INT,
   CTYPE_POINTER,
   CTYPE_FUNCUCTION,
} CTypeType;


typedef struct _CType {
    CTypeType  type;
    CPointer*  pointer;
    CFunction* function;
};


CType* new_integer_ctype();
CType* new_pointer_ctype(CType* to_type);
CType* new_socket_pointer_ctype();
CType* new_function_ctype(Vector* arg_types, CType* return_type);
CType* new_socket_function_ctype(Vector* arg_types);
CType* ctype_copy(CType* ctype);
CType* ctype_connect(CType* socket, CType* plug);
int    ctype_size(CType* ctype);
void   delete_ctype(CType* ctype);

#endif
