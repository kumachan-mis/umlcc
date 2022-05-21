#ifndef _UMLCC_X64_UTIL_H_
#define _UMLCC_X64_UTIL_H_

#include "../set/set.h"
#include "../vector/vector.h"

void append_code(Vector* codes, char* format, ...);
void liveseqs_next(Vector* liveseqs);
Set* create_alive_regs_set(Vector* liveseqs);

#endif
