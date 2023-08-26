#ifndef UMLCC_IMMC_OPE_H
#define UMLCC_IMMC_OPE_H

#include "../literal/iliteral.h"
#include "../literal/sliteral.h"
#include "./register.h"
#include "./suffix.h"

typedef enum ImmcOpeType {
    IMMC_OPERAND_ARG,
    IMMC_OPERAND_REG,
    IMMC_OPERAND_PTR,
    IMMC_OPERAND_MEM,
    IMMC_OPERAND_LABEL,
    IMMC_OPERAND_INT,
    IMMC_OPERAND_STR,
} ImmcOpeType;

typedef struct ImmcOpe {
    ImmcOpeType type;
    ImmcSuffix suffix;
    ImmcRegister* reg;
    int mem_offset;
    char* label_name;
    IntegerLiteral* iliteral;
    StringLiteral* sliteral;
} ImmcOpe;

ImmcOpe* new_arg_immcope(ImmcSuffix suffix, ImmcRegister* reg);
ImmcOpe* new_signed_arg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_unsigned_arg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_reg_immcope(ImmcSuffix suffix, ImmcRegister* reg);
ImmcOpe* new_signed_reg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_unsigned_reg_immcope(ImmcSuffix suffix, int reg_id);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
ImmcOpe* new_label_immcope_from_id(int label_id);
ImmcOpe* new_int_immcope(ImmcSuffix suffix, IntegerLiteral* iliteral);
ImmcOpe* new_signed_int_immcope(ImmcSuffix suffix, IntegerLiteralType type, long long value);
ImmcOpe* new_unsigned_int_immcope(ImmcSuffix suffix, IntegerLiteralType type, unsigned long long value);
ImmcOpe* new_str_immcope(StringLiteral* sliteral);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
void delete_immcope(ImmcOpe* immcope);

#endif
