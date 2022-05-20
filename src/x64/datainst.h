#ifndef _UMLCC_X64_DATAINST_H_
#define _UMLCC_X64_DATAINST_H_

#include "./x64.h"

Vector* gen_load_x64code(X64gen* x64gen);
Vector* gen_addr_x64code(X64gen* x64gen);
Vector* gen_store_x64code(X64gen* x64gen);
Vector* gen_ldarg_x64code(X64gen* x64gen);
Vector* gen_starg_x64code(X64gen* x64gen);
Vector* gen_stret_x64code(X64gen* x64gen);

#endif
