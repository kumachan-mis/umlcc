#ifndef UMLCC_X64_OPE_H
#define UMLCC_X64_OPE_H

#include "./suffix.h"

typedef enum X64OpeType {
    X64_OPERAND_IMM,
    X64_OPERAND_REG,
    X64_OPERAND_PTR,
    X64_OPERAND_MEM,
    X64_OPERAND_LABEL,
    X64_OPERAND_SUFFIX,
} X64OpeType;

typedef struct X64Ope {
    X64OpeType type;
    X64Suffix suffix;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} X64Ope;

X64Ope* new_imm_x64ope(X64Suffix suffix, int imm_value);
X64Ope* new_reg_x64ope(X64Suffix suffix, int reg_id);
X64Ope* new_ptr_x64ope(int reg_id);
X64Ope* new_mem_x64ope(int mem_offset);
X64Ope* new_label_x64ope(char* label_name);
X64Ope* new_suffix_x64ope(X64Suffix suffix);
X64Ope* x64ope_copy(X64Ope* x64ope);
char* x64ope_tostring(X64Ope* x64ope);
void delete_x64ope(X64Ope* x64ope);

#endif
