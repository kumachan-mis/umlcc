#ifndef UMLCC_IMMC_OPE_H
#define UMLCC_IMMC_OPE_H

typedef enum ImmcOpeType {
    IMMC_OPERAND_IMM,
    IMMC_OPERAND_ARG,
    IMMC_OPERAND_REG,
    IMMC_OPERAND_PTR,
    IMMC_OPERAND_MEM,
    IMMC_OPERAND_LABEL
} ImmcOpeType;

typedef enum ImmcSuffix {
    IMMC_SUFFIX_NONE,
    IMMC_SUFFIX_BYTE,
    IMMC_SUFFIX_WORD,
    IMMC_SUFFIX_LONG,
    IMMC_SUFFIX_QUAD
} ImmcSuffix;

typedef struct ImmcOpe {
    ImmcOpeType type;
    ImmcSuffix suffix;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

ImmcOpe* new_imm_immcope(ImmcSuffix suffix, int imm_value);
ImmcOpe* new_arg_immcope(ImmcSuffix suffix, int imm_value);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_reg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
void delete_immope(ImmcOpe* immcope);

ImmcSuffix immcsuffix_get(int size);
int immcsuffix_tosize(ImmcSuffix suffix);
char immcsuffix_tochar(ImmcSuffix suffix);
ImmcSuffix immcsuffix_greater(ImmcSuffix a, ImmcSuffix b);

#endif
