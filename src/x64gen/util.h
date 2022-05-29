#ifndef UMLCC_X64GEN_UTIL_H
#define UMLCC_X64GEN_UTIL_H

#include "../immc/immc.h"
#include "../set/set.h"
#include "../vector/vector.h"

void append_code(Vector* codes, char* format, ...);
void append_mov_code(Vector* codes, int src_reg_id, ImmcOpeSuffix src_immc_suffix, int dst_reg_id,
                     ImmcOpeSuffix dst_immc_suffix);
void liveseqs_next(Vector* liveseqs);
Set* create_alive_regs_set(Vector* liveseqs);
Set* create_arg_regs_set(int num_args);

#endif
