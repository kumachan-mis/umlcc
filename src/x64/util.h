#ifndef UMLCC_X64_UTIL_H
#define UMLCC_X64_UTIL_H

#include "../set/set.h"
#include "../vector/vector.h"

void append_code(Vector* codes, char* format, ...);
void append_conversion_code(Vector* codes, int reg_id, ImmcOpeSuffix from, ImmcOpeSuffix to);
void liveseqs_next(Vector* liveseqs);
Set* create_alive_regs_set(Vector* liveseqs);

#endif
