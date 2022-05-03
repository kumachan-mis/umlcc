#ifndef _UMLCC_IMML_H_
#define _UMLCC_IMML_H_

typedef enum _ImmlInst {
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
    // - store value of fst_src to dest.
    // store dest(reg), fst_src(reg)
    // - store value of fst_src to memory dest points to.
    // store dest(ptr), fst_src(reg)
    INST_LDARG,
    // - load fst_src-th argument of function to dest in memory.
    // ldarg dest(mem), fst_src(imm)
    INST_STARG,
    // - store snd_src to fst_src-th argument of function.
    // starg            fst_src(imm), snd_src(reg)

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

    // control transfar instructions
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

    // label
    INST_GLABEL,
    // - put global label named fst_src
    // glabel           fst_src(label)
    INST_LABEL,
    // - put file-local label named fst_src
    // glabel           fst_src(label)
} ImmlInst;

typedef enum _ImmlOpeType {
    OPERAND_IMM,
    OPERAND_REG,
    OPERAND_PTR,
    OPERAND_MEM,
    OPERAND_LABEL
} ImmlOpeType;

typedef struct _ImmlOpe {
    ImmlOpeType type;
    int imm_value;
    int reg_name;
    int mem_offset;
    char* label_name;
} ImmlOpe;

typedef struct _ImmlCode {
    ImmlInst inst;
    ImmlOpe* dest;
    ImmlOpe* fst_src;
    ImmlOpe* snd_src;
} ImmlCode;

ImmlCode* new_immlcode(ImmlInst inst, ImmlOpe* dest, ImmlOpe* fst_src, ImmlOpe* snd_src);
char* immlcode_tostring(ImmlCode* immlcode);
void delete_immlcode(ImmlCode* immlcode);

ImmlOpe* new_imm_immlope(int imm_value);
ImmlOpe* new_ptr_immlope(int reg_name);
ImmlOpe* new_reg_immlope(int reg_name);
ImmlOpe* new_mem_immlope(int mem_offset);
ImmlOpe* new_label_immlope(char* label_name);
ImmlOpe* immlope_copy(ImmlOpe* immlope);
char* immlope_tostring(ImmlOpe* immlope);
void delete_immope(ImmlOpe* immlope);

#endif
