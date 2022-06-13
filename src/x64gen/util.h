#ifndef UMLCC_X64GEN_UTIL_H
#define UMLCC_X64GEN_UTIL_H

#include "../set/set.h"
#include "../vector/vector.h"
#include "../x64/x64.h"

void append_mov_code(Vector* codes, int src_reg_id, X64Suffix src_suffix, int dst_reg_id,
                     X64Suffix dst_suffix);
void liveseqs_next(Vector* liveseqs);
Set* create_alive_regs_set(Vector* liveseqs);
Set* create_arg_regs_set(int num_args);

#endif
