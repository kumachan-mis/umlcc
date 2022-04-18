#ifndef _UMLCC_TYPE_H_
#define _UMLCC_TYPE_H_

#include "../vector/vector.h"


typedef enum _CTypeType {
   CTYPE_INT,
   CTYPE_FUNC,
} CTypeType;

typedef struct _CType {
    CTypeType      type;
    Vector*        func_args;
    struct _CType* func_return;
} CType;


CType* new_integer_ctype();
CType* new_function_ctype(Vector* func_args, CType* func_return);
void   delete_ctype(CType* ctype);

#endif
