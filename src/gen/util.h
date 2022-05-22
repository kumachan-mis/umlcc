#ifndef _UMLCC_GEN_UTIL_H_
#define _UMLCC_GEN_UTIL_H_

#include "../vector/vector.h"
#include "./gen.h"

void append_children_code(Codegen* codegen, Vector* codes);
void append_child_code(Codegen* codegen, Vector* codes, int index);
ImmcOpeSuffix get_child_suffix(Codegen* codegen, int index);
char* create_label_name(int label_id);

#endif
