#ifndef _UMLCC_GEN_UTIL_H_
#define _UMLCC_GEN_UTIL_H_

#include "../vector/vector.h"
#include "./gen.h"

Vector* gen_children_code(Codegen* codegen);
void append_code(Vector* codes, char* format, ...);

#endif
