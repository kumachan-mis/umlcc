#ifndef UMLCC_IMMC_INST_H
#define UMLCC_IMMC_INST_H

#include "./ope.h"

typedef enum ImmcInstType {
    // - load fst_src value to dst
    // load dst(reg), fst_src(int)
    // - load value from register
    // load dst(reg), fst_src(reg)
    // - load value at memory fst_src points to dst
    // load dst(reg), fst_src(ptr)
    // - load value at fst_src in memory to dst
    // load dst(reg), fst_src(mem)
    // - load value at fst_src label to dst
    // load dst(reg), fst_src(label)
    IMMC_INST_LOAD,
    // - load address of fst_src memory to dst
    // addr dst(reg), fst_src(mem)
    // - load address fst_src label represents
    // addr dst(reg), fst_src(label)
    IMMC_INST_ADDR,
    // - store fst_src string to memory space starting with dst
    // str dst(mem), fst_src(str)
    IMMC_INST_STR,
    // - store fst_src value to memory dst points to
    // store dst(ptr), fst_src(int)
    // - store value of fst_src to memory dst points to
    // store dst(ptr), fst_src(reg)
    // - store fst_src value to dst in memory
    // store dst(mem), fst_src(int)
    // - store value of fst_src to dst in memory
    // store dst(mem), fst_src(reg)
    // - store fst_src value to dst label
    // store dst(label), fst_src(int)
    // - store value of fst_src to dst label
    // store dst(label), fst_src(reg)
    IMMC_INST_STORE,
    // - load fst_src-th argument of function to dst in memory
    // ldarg dst(mem), fst_src(arg)
    IMMC_INST_LDARG,
    // - store fst_src to dst-th argument of function
    // starg dst(arg), fst_src(int)
    // - store fst_src to dst-th argument of function
    // starg dst(arg), fst_src(reg)
    IMMC_INST_STARG,
    // - store fst_src to return value of function
    // stret           fst_src(reg)
    // stret           fst_src(int)
    IMMC_INST_STRET,

    // - add fst_src and snd_src
    // add dst(reg), fst_src(reg), snd_src(int)
    // - add fst_src and snd_src
    // add dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_ADD,
    // - subtract snd_src from fst_src
    // sub dst(reg), fst_src(reg), snd_src(int)
    // - subtract snd_src from fst_src
    // sub dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SUB,
    // - multiply fst_src and snd_src
    // mul dst(reg), fst_src(reg), snd_src(int)
    // - multiply fst_src and snd_src
    // mul dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_MUL,
    // - divide fst_src by snd_src
    // div dst(reg), fst_src(reg), snd_src(int)
    // - divide fst_src by snd_src
    // div dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_DIV,
    // - modulo fst_src by snd_src
    // mod dst(reg), fst_src(reg), snd_src(int)
    // - modulo fst_src by snd_src
    // mod dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_MOD,
    // - nagate fst_src
    // neg dst(reg), fst_src(int)
    // - nagate fst_src
    // neg dst(reg), fst_src(reg)
    IMMC_INST_NEG,

    // - bitwise and fst_src and snd_src
    // and dst(reg), fst_src(reg), snd_src(int)
    // - bitwise and fst_src and snd_src
    // and dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_AND,
    // - bitwise not fst_src
    // not dst(reg), fst_src(int)
    // - bitwise not fst_src
    // not dst(reg), fst_src(reg)
    IMMC_INST_NOT,
    // - bitwise or fst_src and snd_src
    // or dst(reg), fst_src(reg), snd_src(int)
    // - bitwise or fst_src and snd_src
    // or dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_OR,
    // - bitwise xor fst_src and snd_src
    // xor dst(reg), fst_src(reg), snd_src(int)
    // - bitwise xor fst_src and snd_src
    // xor dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_XOR,

    // - arithmetic shift-left fst_src to snd_src bit positions
    // sal dst(reg), fst_src(reg), snd_src(int)
    IMMC_INST_SAL,
    // - arithmetic shift-right fst_src to snd_src bit positions
    // sar dst(reg), fst_src(reg), snd_src(int)
    IMMC_INST_SAR,

    // - set dst to 1 if fst_src equals to snd_src, 0 otherwise
    // seteq dst(reg), fst_src(reg), snd_src(int)
    // - set dst to 1 if fst_src equals to snd_src, 0 otherwise
    // seteq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETEQ,
    // - set dst to 1 if fst_src does not equal to snd_src, 0 otherwise
    // setneq dst(reg), fst_src(reg), snd_src(int)
    // - set dst to 1 if fst_src does not equal to snd_src, 0 otherwise
    // setneq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETNEQ,
    // - set dst to 1 if fst_src is less than snd_src, 0 otherwise
    // setlt dst(reg), fst_src(reg), snd_src(int)
    // - set dst to 1 if fst_src is less than snd_src, 0 otherwise
    // setlt dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETLT,
    // - set dst to 1 if fst_src is greater than snd_src, 0 otherwise
    // setgt dst(reg), fst_src(reg), snd_src(int)
    // - set dst to 1 if fst_src is greater than snd_src, 0 otherwise
    // setgt dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETGT,
    // - set dst to 1 if fst_src is less than or equal to snd_src, 0 otherwise
    // setleq dst(reg), fst_src(reg), snd_src(int)
    // - set dst to 1 if fst_src is less than or equal to snd_src, 0 otherwise
    // setleq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETLEQ,
    // - set dst to 1 if fst_src is greater than or equal to snd_src, 0 otherwise
    // setgeq dst(reg), fst_src(reg), snd_src(int)
    // - set dst to 1 if fst_src is greater than or equal to snd_src, 0 otherwise
    // setgeq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_SETGEQ,

    // - jump to dst label
    // jmp dst(label)
    IMMC_INST_JMP,
    // - jump to dst label if fst_src equals to snd_src
    // jeq dst(label), fst_src(reg), snd_src(int)
    // - jump to dst label if fst_src equals to snd_src
    // jeq dst(label), fst_src(reg), snd_src(reg)
    IMMC_INST_JEQ,
    // - jump to dst label if fst_src does not equal to snd_src
    // jneq dst(label), fst_src(reg), snd_src(int)
    // - jump to dst label if fst_src does not equal to snd_src
    // jneq dst(label), fst_src(reg), snd_src(reg)
    IMMC_INST_JNEQ,
    // - jump to dst label if fst_src is greater than snd_src
    // jgt dst(reg), fst_src(reg), snd_src(int)
    // - jump to dst label if fst_src is greater than snd_src
    // jgt dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_JGT,
    // - jump to dst label if fst_src is greater than or equal to snd_src
    // jgeq dst(reg), fst_src(reg), snd_src(int)
    // - jump to dst label if fst_src is greater than or equal to snd_src
    // jgeq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_JGEQ,
    // - jump to dst label if fst_src is less than snd_src
    // jlt dst(reg), fst_src(reg), snd_src(int)
    // - jump to dst label if fst_src is less than snd_src
    // jlt dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_JLT,
    // - jump to dst label if fst_src is less than or equal to snd_src
    // jleq dst(reg), fst_src(reg), snd_src(int)
    // - jump to dst label if fst_src is less than or equal to snd_src
    // jleq dst(reg), fst_src(reg), snd_src(reg)
    IMMC_INST_JLEQ,
    // - call function at address of fst_src, which takes snd_src-params
    //   (return value is stored to dst)
    // call dst(reg), fst_src(ptr), snd_src(int)
    // - call function at address of fst_src, which takes snd_src-params
    //   (return value is nothing)
    // call           fst_src(ptr), snd_src(int)
    // - call function named fst_src, which takes snd_src-params
    //   (return value is stored to dst)
    // call dst(reg), fst_src(label), snd_src(int)
    // - call function named fst_src, which takes snd_src-params
    //   (return value is nothing)
    // call           fst_src(label), snd_src(int)
    IMMC_INST_CALL,
    // - enter function which needs fst_src-bytes local memory
    // enter           fst_src(int)
    IMMC_INST_ENTER,
    // - leave function which needed fst_src-bytes local memory
    // leave           fst_src(int)
    IMMC_INST_LEAVE,

    // - prepare for function call which takes fst_src-params
    //   immediate code DOES NOT define what to do in this instruction
    //   it gives chance to do "anything" before setting argments to object code
    // prep           fst_src(int)
    IMMC_INST_PREP,
    // - clean up for function call which takes fst_src-params
    //   immediate code DOES NOT define what to do in this instruction
    //   it gives chance to do "anything" after calling function to object code
    // clean           fst_src(int)
    IMMC_INST_CLEAN,
} ImmcInstType;

typedef struct ImmcInst {
    ImmcInstType type;
    ImmcOpe* dst;
    ImmcOpe* fst_src;
    ImmcOpe* snd_src;
} ImmcInst;

#ifdef UMLCC_IMMC_H_PRIVATE
ImmcInst* new_immcinst(ImmcInstType type, ImmcOpe* dst, ImmcOpe* fst_src, ImmcOpe* snd_src);
ImmcInst* immcinst_copy(ImmcInst* immcinst);
char* immcinst_tostring(ImmcInst* immcinst);
void delete_immcinst(ImmcInst* immcinst);
#endif

#endif
