#ifndef UMLCC_IMMC_INST_H
#define UMLCC_IMMC_INST_H

#include "./ope.h"

typedef enum ImmcInstType {
    // data transfar instructions
    IMMC_INST_LOAD,
    // - load fst_src value to dst.
    // load dst(reg), fst_src(imm)
    // - load value at memory fst_src points to dst.
    // load dst(reg), fst_src(ptr)
    // - load value at fst_src in memory to dst.
    // load dst(reg), fst_src(mem)
    // - load value at fst_src label to dst.
    // load dst(reg), fst_src(label)
    IMMC_INST_ADDR,
    // - load address of fst_src memory to dst.
    // addr dst(reg), fst_src(mem)
    // - load address fst_src label represents.
    // addr dst(reg), fst_src(label)
    IMMC_INST_STORE,
    // - store value of fst_src to memory dst points to.
    // store dst(ptr), fst_src(reg)
    // - store value of fst_src to dst in memory.
    // store dst(mem), fst_src(reg)
    IMMC_INST_LDARG,
    // - load fst_src-th argument of function to dst in memory.
    // ldarg dst(mem), fst_src(arg)
    IMMC_INST_STARG,
    // - store snd_src to fst_src-th argument of function.
    // starg            fst_src(arg), snd_src(reg)
    IMMC_INST_STRET,
    // - store fst_src to return value of function.
    // stret           fst_src(reg)

    // arithmetic instructions
    IMMC_INST_ADD,
    // - add fst_src and snd_src.
    // add dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SUB,
    // - subtract snd_src from fst_src.
    // sub dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_MUL,
    // - multiply fst_src and snd_src.
    // mul dst(reg), fst_src(reg), snd_src(reg)
    // - multiply fst_src and snd_src.
    // mul dst(reg), fst_src(reg), snd_src(imm)
    IMMC_INST_DIV,
    // - divide fst_src by snd_src.
    // div dst(reg), fst_src(reg), snd_src(reg)
    // - divide fst_src by snd_src.
    // div dst(reg), fst_src(reg), snd_src(imm)
    IMMC_INST_MOD,
    // - modulo fst_src by snd_src.
    // mod dst(reg), fst_src(reg), snd_src(reg)
    // - modulo fst_src by snd_src.
    // mod dst(reg), fst_src(reg), snd_src(imm)

    // shift/rotate instructions
    IMMC_INST_SAL,
    // - arithmetic shift-left fst_src to snd_src bit positions
    // sal dst(reg), fst_src(reg), snd_src(imm)
    IMMC_INST_SAR,
    // - arithmetic shift-right fst_src to snd_src bit positions
    // sar dst(reg), fst_src(reg), snd_src(imm)

    // bit/byte instructions
    IMMC_INST_SETEQ,
    // - set dst to 1 if fst_src equals to snd_src, 0 otherwise.
    // seteq dst(reg), fst_src(reg), snd_src(imm)
    // seteq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETNEQ,
    // - set dst to 1 if fst_src does not equal to snd_src, 0 otherwise.
    // setneq dst(reg), fst_src(reg), snd_src(imm)
    // setneq dst(reg), fst_src(reg), snd_src(reg)

    // control transfar instructions
    IMMC_INST_JMP,
    // - jump to dst label.
    // jmp dst(label)
    IMMC_INST_JEQ,
    // - jump to dst label if fst_src equals to snd_src.
    // jeq dst(label), fst_src(reg), snd_src(imm)
    // jeq dst(label), fst_src(reg), snd_src(reg)
    IMMC_INST_JNEQ,
    // - jump to dst label if fst_src does not equal to snd_src.
    // jneq dst(label), fst_src(reg), snd_src(imm)
    // jneq dst(label), fst_src(reg), snd_src(reg)
    IMMC_INST_CALL,
    // - call function at address of fst_src, which takes snd_src-params.
    //   (return value is stored to dst)
    // call dst(reg), fst_src(ptr), snd_src(imm)
    IMMC_INST_ENTER,
    // - enter function which needs fst_src-bytes local memory.
    // enter           fst_src(imm)
    IMMC_INST_LEAVE,
    // - leave function which needed fst_src-bytes local memory.
    // leave           fst_src(imm)

    // misc instructions
    IMMC_INST_PREP,
    // - prepare for function call which takes fst_src-params.
    //   immediate code DOES NOT define what to do in this instruction.
    //   it gives chance to do "anything" before setting argments to object code.
    // prep           fst_src(imm)
    IMMC_INST_CLEAN,
    // - clean up for function call which takes fst_src-params.
    //   immediate code DOES NOT define what to do in this instruction.
    //   it gives chance to do "anything" after calling function to object code.
    // clean           fst_src(imm)
} ImmcInstType;

typedef struct ImmcInst {
    ImmcInstType type;
    ImmcOpe* dst;
    ImmcOpe* fst_src;
    ImmcOpe* snd_src;
} ImmcInst;

ImmcInst* new_immcinst(ImmcInstType type, ImmcOpe* dst, ImmcOpe* fst_src, ImmcOpe* snd_src);
ImmcInst* immcinst_copy(ImmcInst* immcinst);
int immcinst_isjump(ImmcInst* immcinst);
char* immcinst_tostring(ImmcInst* immcinst);
void delete_immcinst(ImmcInst* immcinst);

#endif
