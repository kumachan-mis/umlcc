#ifndef _UMLCC_TYPE_FUNCTION_H_
#define _UMLCC_TYPE_FUNCTION_H_

#include "../vector/vector.h"


typedef struct _CType CType;

typedef struct _CFunction {
    Vector* params;
    CType*  return_ctype;
} CFunction;

typedef struct _CParam {
    char* ident_name;
    CType* ctype;
} CParam;

CFunction* new_cfunction(Vector* params, CType* return_ctype);
CFunction* new_socket_cfunction(Vector* params);
CFunction* cfunction_copy(CFunction* cfunction);
CType*     cfunction_next(CFunction* cfunction);
CFunction* cfunction_connect(CFunction* socket, CType* plug);
void       delete_cfunction(CFunction* cfunction);
CParam*    new_cparam(char* ident_name, CType* ctype);
CParam*    cparam_copy(CParam* cparam);
void       delete_cparam(CParam* cparam);

#endif
