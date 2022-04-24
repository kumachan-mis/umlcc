#ifndef _UMLCC_TYPE_FUNCTION_H_
#define _UMLCC_TYPE_FUNCTION_H_

#include "../vector/vector.h"


typedef struct _CType CType;

typedef struct _CFunction {
    Vector* arg_types;
    CType*  return_type;
} CFunction;

CFunction* new_cfunction(Vector* arg_types, CType* return_type);
CFunction* new_socket_cfunction(Vector* arg_types);
CFunction* cfunction_copy(CFunction* cfunction);
CType*     cfunction_next(CFunction* cfunction);
CFunction* cfunction_connect(CFunction* socket, CType* plug);
void       delete_cfunction(CFunction* cfunction);

#endif
