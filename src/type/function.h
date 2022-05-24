#ifndef UMLCC_TYPE_FUNCTION_H
#define UMLCC_TYPE_FUNCTION_H

#include "../common/common.h"
#include "../vector/vector.h"

typedef struct CFunction {
    Vector* params;
    struct CType* return_ctype;
} CFunction;

typedef struct CParam {
    char* ident_name;
    struct CType* ctype;
} CParam;

extern BaseType t_cparam;

CFunction* new_cfunction(Vector* params, struct CType* return_ctype);
CFunction* new_socket_cfunction(Vector* params);
CFunction* cfunction_copy(CFunction* cfunction);
struct CType* cfunction_next(CFunction* cfunction);
CFunction* cfunction_connect(CFunction* socket, struct CType* plug);
void delete_cfunction(CFunction* cfunction);

CParam* new_cparam(char* ident_name, struct CType* ctype);
CParam* cparam_copy(CParam* cparam);
void delete_cparam(CParam* cparam);

#endif
