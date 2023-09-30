#ifndef UMLCC_X64_INST_H
#define UMLCC_X64_INST_H

#include "./ope.h"

typedef enum X64InstType {
    // data transfar instructions
    X64_INST_MOVX,
    X64_INST_MOVSXX,
    X64_INST_MOVZXX,
    X64_INST_MOVABSX,
    X64_INST_LEAX,
    X64_INST_CXTX,
    X64_INST_CXTD,
    X64_INST_CQTO,
    X64_INST_PUSHX,
    X64_INST_POPX,

    // arithmetic instructions
    X64_INST_ADDX,
    X64_INST_SUBX,
    X64_INST_IMULX,
    X64_INST_DIVX,
    X64_INST_IDIVX,
    X64_INST_NEGX,

    // logical instructions
    X64_INST_ANDX,
    X64_INST_NOTX,
    X64_INST_ORX,
    X64_INST_XORX,

    // shift/rotate instructions
    X64_INST_SALX,
    X64_INST_SARX,

    // bit/byte instructions
    X64_INST_CMPX,
    X64_INST_SETE,
    X64_INST_SETNE,
    X64_INST_SETA,
    X64_INST_SETAE,
    X64_INST_SETB,
    X64_INST_SETBE,
    X64_INST_SETG,
    X64_INST_SETGE,
    X64_INST_SETL,
    X64_INST_SETLE,

    // control transfar instructions
    X64_INST_JMP,
    X64_INST_JE,
    X64_INST_JNE,
    X64_INST_JA,
    X64_INST_JAE,
    X64_INST_JB,
    X64_INST_JBE,
    X64_INST_JG,
    X64_INST_JGE,
    X64_INST_JL,
    X64_INST_JLE,
    X64_INST_CALL,
    X64_INST_RET,
} X64InstType;

typedef struct X64Inst {
    X64InstType type;
    X64Ope* src;
    X64Ope* dst;
} X64Inst;

#ifdef UMLCC_X64_H_PRIVATE
X64Inst* new_x64inst(X64InstType type, X64Ope* src, X64Ope* dst);
X64Inst* x64inst_copy(X64Inst* x64inst);
char* x64inst_tostring(X64Inst* x64inst);
void delete_x64inst(X64Inst* x64inst);
#endif

#endif
