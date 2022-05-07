#ifndef _UMLCC_GEN_UTIL_H_
#define _UMLCC_GEN_UTIL_H_

#include "../vector/vector.h"
#include "./gen.h"

Vector* append_children_code(Codegen* codegen, Vector* codes);
Vector* append_child_code(Codegen* codegen, Vector* codes, int index);

#endif
