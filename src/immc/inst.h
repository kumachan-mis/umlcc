#ifndef _UMLCC_IMMC_INST_H_
#define _UMLCC_IMMC_IIST_H_

typedef enum _ImmcInstType {
    // data transfar instructions
    INST_LOAD,
    // - load fst_src value to dest.
    // load dest(reg), fst_src(imm)
    // - load value at fst_src in memory to dest.
    // load dest(reg), fst_src(mem)
    // - load value at fst_src label to dest.
    // load dest(reg), fst_src(label)
    INST_ADDR,
    // - load address of fst_src memory to dest.
    // addr dest(reg), fst_src(mem)
    // - load address fst_src label represents.
    // addr dest(reg), fst_src(label)
    INST_STORE,
    // - store value of fst_src to memory dest points to.
    // store dest(ptr), fst_src(reg)
    INST_LDARG,
    // - load fst_src-th argument of function to dest in memory.
    // ldarg dest(mem), fst_src(imm)
    INST_STARG,
    // - store snd_src to fst_src-th argument of function.
    // starg            fst_src(imm), snd_src(reg)
    INST_STRET,
    // - store fst_src to return value of function.
    // stret           fst_src(reg)

    // arithmetic instructions
    INST_ADD,
    // - add fst_src and snd_src.
    // add dest(reg), fst_src(reg), snd_src(reg)
    INST_SUB,
    // - subtract snd_src from fst_src.
    // sub dest(reg), fst_src(reg), snd_src(reg)
    INST_MUL,
    // - multiply fst_src and snd_src.
    // mul dest(reg), fst_src(reg), snd_src(reg)
    INST_DIV,
    // - divide fst_src by snd_src.
    // div dest(reg), fst_src(reg), snd_src(reg)
    INST_MOD,
    // - modulo fst_src by snd_src.
    // mod dest(reg), fst_src(reg), snd_src(reg)

    // bit/byte instructions
    INST_SETEQ,
    // - set dest to 1 if fst_src equals to snd_src, 0 otherwise.
    // seteq dest(reg), fst_src(reg), snd_src(imm)
    // seteq dest(reg), fst_src(reg), snd_src(reg)

    INST_SETNEQ,
    // - set dest to 1 if fst_src does not equal to snd_src, 0 otherwise.
    // setneq dest(reg), fst_src(reg), snd_src(imm)
    // setneq dest(reg), fst_src(reg), snd_src(reg)

    // control transfar instructions
    INST_JMP,
    // - jump to dest label.
    // jmp dest(label)
    INST_JEQ,
    // - jump to dest label if fst_src equals to snd_src.
    // jeq dest(label), fst_src(reg), snd_src(imm)
    // jeq dest(label), fst_src(reg), snd_src(reg)
    INST_JNEQ,
    // - jump to dest label if fst_src does not equal to snd_src.
    // jneq dest(label), fst_src(reg), snd_src(imm)
    // jneq dest(label), fst_src(reg), snd_src(reg)
    INST_CALL,
    // - call function at address of fst_src, which takes snd_src-params.
    //   (return value is stored to dest)
    // call dest(reg), fst_src(ptr), snd_src(imm)
    INST_ENTER,
    // - enter function which needs fst_src-bytes local memory.
    // enter           fst_src(imm)
    INST_LEAVE,
    // - leave function which needed fst_src-bytes local memory.
    // leave           fst_src(imm)

    // misc instructions
    INST_PREP,
    // - prepare for function call which takes fst_src-params.
    //   immediate code DOES NOT define what to do in this instruction.
    //   it gives chance to do "anything" before setting argments to object code.
    // prep           fst_src(imm)
    INST_CLEAN,
    // - clean up for function call which takes fst_src-params.
    //   immediate code DOES NOT define what to do in this instruction.
    //   it gives chance to do "anything" after calling function to object code.
    // clean           fst_src(imm)
    INST_FREE,
    // - force to free fst_src register.
    // free           fst_src(reg)
} ImmcInstType;

typedef enum _ImmcOpeType {
    OPERAND_IMM,
    OPERAND_REG,
    OPERAND_PTR,
    OPERAND_MEM,
    OPERAND_LABEL
} ImmcOpeType;

typedef struct _ImmcOpe {
    ImmcOpeType type;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

typedef struct _ImmcInst {
    ImmcInstType type;
    ImmcOpe* dest;
    ImmcOpe* fst_src;
    ImmcOpe* snd_src;
} ImmcInst;

ImmcInst* new_immcinst(ImmcInstType type, ImmcOpe* dest, ImmcOpe* fst_src, ImmcOpe* snd_src);
char* immcinst_tostring(ImmcInst* immcinst);
void delete_immcinst(ImmcInst* immcinst);

ImmcOpe* new_imm_immcope(int imm_value);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_reg_immcope(int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
void delete_immope(ImmcOpe* immcope);

#endif
