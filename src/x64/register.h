#ifndef UMLCC_X64GEN_REGISTER_H
#define UMLCC_X64GEN_REGISTER_H

#include "./suffix.h"

typedef enum X64RegisterId {
    AX_REG_ID,
    BX_REG_ID,
    CX_REG_ID,
    DX_REG_ID,
    SP_REG_ID,
    BP_REG_ID,
    SI_REG_ID,
    DI_REG_ID,
    R8_REG_ID,
    R9_REG_ID,
    R10_REG_ID,
    R11_REG_ID,
    R12_REG_ID,
    R13_REG_ID,
    R14_REG_ID,
    R15_REG_ID,
} X64RegisterId;

extern int NUM_ARG_REGS;
extern int ARG_REG_IDS[6];

extern int NUM_CALLER_SAVED_REGS;
extern int CALLER_SAVED_REG_IDS[8];

extern int NUM_CALLEE_SAVED_REGS;
extern int CALLEE_SAVED_REG_IDS[5];

extern char* PC_NAME;

char* reg_name(X64RegisterId reg_id, X64Suffix suffix);

#endif
