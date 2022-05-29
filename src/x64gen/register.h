#ifndef UMLCC_X64GEN_REGISTER_H
#define UMLCC_X64GEN_REGISTER_H

#include "../immc/immc.h"

extern char* PC_NAME;
extern char* BREG_NAMES[16];
extern char* WREG_NAMES[16];
extern char* LREG_NAMES[16];
extern char* QREG_NAMES[16];

extern int ARG_REG_IDS[6];
extern int CALLER_SAVED_REG_IDS[8];
extern int CALLEE_SAVED_REG_IDS[5];

extern int AX_REG_ID;
extern int BX_REG_ID;
extern int CX_REG_ID;
extern int DX_REG_ID;
extern int SP_REG_ID;
extern int BP_REG_ID;
extern int SI_REG_ID;
extern int DI_REG_ID;
extern int R8_REG_ID;
extern int R9_REG_ID;
extern int R10_REG_ID;
extern int R11_REG_ID;
extern int R12_REG_ID;
extern int R13_REG_ID;
extern int R14_REG_ID;
extern int R15_REG_ID;

extern int NUM_REGS;
extern int NUM_ARG_REGS;
extern int NUM_CALLER_SAVED_REGS;
extern int NUM_CALLEE_SAVED_REGS;

char* reg_name(int reg_id, ImmcOpeSuffix suffix);

#endif
