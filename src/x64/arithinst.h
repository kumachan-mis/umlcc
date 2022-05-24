#ifndef UMLCC_X64_ARITHINST_H
#define UMLCC_X64_ARITHINST_H

#include "./x64.h"

Vector* gen_add_x64code(X64gen* x64gen);
Vector* gen_sub_x64code(X64gen* x64gen);
Vector* gen_mul_x64code(X64gen* x64gen);
Vector* gen_div_x64code(X64gen* x64gen);
Vector* gen_mod_x64code(X64gen* x64gen);

#endif
