#ifndef _UMLCC_X64_CTRLINST_H_
#define _UMLCC_X64_CTRLINST_H_

#include "./x64.h"

Vector* gen_jmp_x64code(X64gen* x64gen);
Vector* gen_jeq_x64code(X64gen* x64gen);
Vector* gen_jneq_x64code(X64gen* x64gen);
Vector* gen_call_x64code(X64gen* x64gen);
Vector* gen_enter_x64code(X64gen* x64gen);
Vector* gen_leave_x64code(X64gen* x64gen);
Vector* gen_prep_x64code(X64gen* x64gen);
Vector* gen_clean_x64code(X64gen* x64gen);

#endif
