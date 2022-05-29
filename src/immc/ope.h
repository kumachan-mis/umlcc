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

typedef enum ImmcOpeSuffix {
    IMMC_SUFFIX_NONE,
    IMMC_SUFFIX_BYTE,
    IMMC_SUFFIX_WORD,
    IMMC_SUFFIX_LONG,
    IMMC_SUFFIX_QUAD
} ImmcOpeSuffix;

typedef struct ImmcOpe {
    ImmcOpeType type;
    ImmcOpeSuffix suffix;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

ImmcOpe* new_imm_immcope(int imm_value);
ImmcOpe* new_arg_immcope(ImmcOpeSuffix suffix, int imm_value);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_reg_immcope(ImmcOpeSuffix suffix, int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
void delete_immope(ImmcOpe* immcope);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
ImmcOpeSuffix immcope_suffix_get(int size);
int immcope_suffix_tosize(ImmcOpeSuffix suffix);
char immcope_suffix_tochar(ImmcOpeSuffix suffix);
ImmcOpeSuffix immcope_suffix_max(ImmcOpeSuffix a, ImmcOpeSuffix b);

#endif
