#ifndef _UMLCC_TYPE_FUNCTION_H_
#define _UMLCC_TYPE_FUNCTION_H_

#include "../vector/vector.h"

typedef struct _CFunction {
    Vector* params;
    struct _CType* return_ctype;
} CFunction;

typedef struct _CParam {
    char* ident_name;
    struct _CType* ctype;
} CParam;

CFunction* new_cfunction(Vector* params, struct _CType* return_ctype);
CFunction* new_socket_cfunction(Vector* params);
CFunction* cfunction_copy(CFunction* cfunction);
struct _CType* cfunction_next(CFunction* cfunction);
CFunction* cfunction_connect(CFunction* socket, struct _CType* plug);
void delete_cfunction(CFunction* cfunction);
CParam* new_cparam(char* ident_name, struct _CType* ctype);
CParam* cparam_copy(CParam* cparam);
void delete_cparam(CParam* cparam);

#endif
