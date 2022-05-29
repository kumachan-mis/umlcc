#ifndef UMLCC_X64GEN_DATAINST_H
#define UMLCC_X64GEN_DATAINST_H

#include "./x64gen.h"

Vector* gen_load_x64code(X64gen* x64gen);
Vector* gen_addr_x64code(X64gen* x64gen);
Vector* gen_store_x64code(X64gen* x64gen);
Vector* gen_ldarg_x64code(X64gen* x64gen);
Vector* gen_starg_x64code(X64gen* x64gen);
Vector* gen_stret_x64code(X64gen* x64gen);

#endif
