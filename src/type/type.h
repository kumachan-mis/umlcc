#ifndef _UMLCC_TYPE_H_
#define _UMLCC_TYPE_H_

#include "../common/common.h"
#include "../vector/vector.h"
#include "./function.h"
#include "./pointer.h"

typedef enum _CTypeType {
    CTYPE_INT,
    CTYPE_POINTER,
    CTYPE_FUNCUCTION,
} CTypeType;

typedef struct _CType {
    CTypeType type;
    CPointer* pointer;
    CFunction* function;
} CType;

extern BaseType t_ctype;

CType* new_integer_ctype();
CType* new_pointer_ctype(CType* to_ctype);
CType* new_socket_pointer_ctype();
CType* new_function_ctype(Vector* params, CType* return_ctype);
CType* new_socket_function_ctype(Vector* params);
CType* ctype_copy(CType* ctype);
CType* ctype_connect(CType* socket, CType* plug);
int ctype_isarithmetic(CType* ctype);
int ctype_size(CType* ctype);
void delete_ctype(CType* ctype);

#endif
