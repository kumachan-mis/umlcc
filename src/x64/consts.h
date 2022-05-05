#ifndef _UMLCC_X64_CONSTS_H_
#define _UMLCC_X64_CONSTS_H_

extern char* PC_NAME;
extern char* QREG_NAMES[16];
extern char* LREG_NAMES[16];

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

extern int NUM_ARGREGS;

char* arg_reg_name(char* reg_names[16], int index);
char* callee_reg_name(char* reg_names[16], int index);

#endif
