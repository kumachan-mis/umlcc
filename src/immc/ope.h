#ifndef UMLCC_IMMC_OPE_H
#define UMLCC_IMMC_OPE_H

#include "./suffix.h"

typedef enum ImmcOpeType {
    IMMC_OPERAND_IMM,
    IMMC_OPERAND_ARG,
    IMMC_OPERAND_REG,
    IMMC_OPERAND_PTR,
    IMMC_OPERAND_MEM,
    IMMC_OPERAND_LABEL
} ImmcOpeType;

typedef struct ImmcOpe {
    ImmcOpeType type;
    ImmcSuffix suffix;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

ImmcOpe* new_imm_immcope(int imm_value);
ImmcOpe* new_arg_immcope(ImmcSuffix suffix, int imm_value);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_reg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
void delete_immcope(ImmcOpe* immcope);

#endif
