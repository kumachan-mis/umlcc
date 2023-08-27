#ifndef UMLCC_X64GEN_BITINST_H
#define UMLCC_X64GEN_BITINST_H

#include "./x64gen.h"

Vector* gen_seteq_x64code(X64gen* x64gen);
Vector* gen_setneq_x64code(X64gen* x64gen);
Vector* gen_setlt_x64code(X64gen* x64gen);
Vector* gen_setgt_x64code(X64gen* x64gen);
Vector* gen_setleq_x64code(X64gen* x64gen);
Vector* gen_setgeq_x64code(X64gen* x64gen);

#endif
