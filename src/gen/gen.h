#ifndef _UMLCC_GEN_H_
#define _UMLCC_GEN_H_

#include "../vector/vector.h"


typedef struct _Codegen Codegen;

Codegen* new_codegen(Vector* asts);
void     delete_codegen(Codegen* codegen);
Vector*  codegen_generate_code(Codegen* codegen);

#endif