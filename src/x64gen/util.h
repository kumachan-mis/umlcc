#ifndef UMLCC_X64GEN_UTIL_H
#define UMLCC_X64GEN_UTIL_H

#include "../set/set.h"
#include "../vector/vector.h"
#include "../x64/x64.h"

void append_mov_code(Vector* codes, X64Ope* src, X64Ope* dst, int is_unsigned);
void liveseqs_next(Vector* liveseqs);
Set* create_alive_reg_induces_set(Vector* liveseqs);
Set* create_alive_regs_set(Vector* liveseqs);
Set* create_arg_regs_set(int num_args);

#endif
