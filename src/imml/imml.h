#ifndef _UMLCC_IMML_H_
#define _UMLCC_IMML_H_

typedef enum _ImmlInst {
    // data transfar instructions
    INST_LOAD,
    // load data.
    // load dest{reg}, fst_src{imm,mem,label}
    INST_ADDR,
    // load affress.
    // addr dest{reg}, fst_src{mem,label}
    INST_STORE,
    // store data.
    // store dest{mem}, fst_src{reg}

    // arithmetic instructions
    INST_ADD,
    // add.
    // add dest{reg}, fst_src{reg}, snd_src{reg}
    INST_SUB,
    // subtract.
    // sub dest{reg}, fst_src{reg}, snd_src{reg}
    INST_MUL,
    // multiply.
    // mul dest{reg}, fst_src{reg}, snd_src{reg}
    INST_DIV,
    // divide.
    // div dest{reg}, fst_src{reg}, snd_src{reg}
    INST_MOD,
    // modulo.
    // mod dest{reg}, fst_src{reg}, snd_src{reg}

    // control transfar instructions
    INST_CALL,
    // call function at address, n-params.
    // call fst_src{reg} snd_src{imm}
    INST_ARG,
    // set n-th argument for function.
    // arg fst_src{imm} snd_src{reg}

    // label
    INST_LABEL,
    // put label.
    // label src{label}
} ImmlInst;

typedef enum _ImmlOpeType { OPERAND_IMM, OPERAND_REG, OPERAND_MEM, OPERAND_LABEL } ImmlOpeType;

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
void delete_immlcode(ImmlCode* immlcode);
ImmlOpe* new_imm_immlope(int imm_value);
ImmlOpe* new_reg_immlope(int reg_name);
ImmlOpe* new_mem_immlope(int mem_offset);
ImmlOpe* new_label_immlope(char* label_name);
void delete_immope(ImmlOpe* immlope);

#endif
