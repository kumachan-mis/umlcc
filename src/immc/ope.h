#ifndef UMLCC_IMMC_OPE_H
#define UMLCC_IMMC_OPE_H

#include "./suffix.h"

typedef enum ImmcOpeType {
    IMMC_OPERAND_IMM,
    IMMC_OPERAND_ARG,
    IMMC_OPERAND_REG,
    IMMC_OPERAND_PTR,
    IMMC_OPERAND_MEM,
    IMMC_OPERAND_LABEL,
    IMMC_OPERAND_STR,
} ImmcOpeType;

typedef struct ImmcOpe {
    ImmcOpeType type;
    ImmcSuffix suffix;
    int imm_value;
    char* str_value;
    int str_size;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

ImmcOpe* new_imm_immcope(ImmcSuffix suffix, int imm_value);
ImmcOpe* new_arg_immcope(ImmcSuffix suffix, int imm_value);
ImmcOpe* new_reg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
ImmcOpe* new_str_immcope(char* str_value, int str_size);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
void delete_immcope(ImmcOpe* immcope);

#endif
