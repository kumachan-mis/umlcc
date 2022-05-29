#ifndef UMLCC_IMMCGEN_UTIL_H
#define UMLCC_IMMCGEN_UTIL_H

#include "../vector/vector.h"
#include "./immcgen.h"

void append_children_immcode(Immcgen* immcgen, Vector* codes);
void append_child_immcode(Immcgen* immcgen, Vector* codes, int index);
char* create_label_name(int label_id);

#endif
