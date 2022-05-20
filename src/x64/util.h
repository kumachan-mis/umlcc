#ifndef _UMLCC_X64_UTIL_H_
#define _UMLCC_X64_UTIL_H_

#include "../vector/vector.h"

void append_code(Vector* codes, char* format, ...);
void liveseqs_next(Vector* liveseqs);

#endif
