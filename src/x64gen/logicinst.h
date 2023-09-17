#ifndef UMLCC_X64GEN_LOGICINST_H
#define UMLCC_X64GEN_LOGICINST_H

#include "./x64gen.h"

Vector* gen_and_x64code(X64gen* x64gen);
Vector* gen_not_x64code(X64gen* x64gen);
Vector* gen_or_x64code(X64gen* x64gen);
Vector* gen_xor_x64code(X64gen* x64gen);

#endif
