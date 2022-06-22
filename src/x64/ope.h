#ifndef UMLCC_X64_OPE_H
#define UMLCC_X64_OPE_H

#include "./suffix.h"

typedef enum X64OpeType {
    X64_OPERAND_SIGNED,
    X64_OPERAND_UNSIGNED,
    X64_OPERAND_REG,
    X64_OPERAND_PTR,
    X64_OPERAND_JPTR,
    X64_OPERAND_MEM,
    X64_OPERAND_LABEL,
    X64_OPERAND_JLABEL,
    X64_OPERAND_SUFFIX,
} X64OpeType;

typedef struct X64Ope {
    X64OpeType type;
    X64Suffix suffix;
    long long signed_value;
    unsigned long long unsigned_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} X64Ope;

X64Ope* new_signed_x64ope(X64Suffix suffix, long long signed_value);
X64Ope* new_unsigned_x64ope(X64Suffix suffix, unsigned long long unsigned_value);
X64Ope* new_reg_x64ope(X64Suffix suffix, int reg_id);
X64Ope* new_ptr_x64ope(int reg_id);
X64Ope* new_jptr_x64ope(int reg_id);
X64Ope* new_mem_x64ope(int mem_offset);
X64Ope* new_label_x64ope(char* label_name);
X64Ope* new_jlabel_x64ope(char* label_name);
X64Ope* new_suffix_x64ope(X64Suffix suffix);
X64Ope* x64ope_copy(X64Ope* x64ope);
char* x64ope_tostring(X64Ope* x64ope);
void delete_x64ope(X64Ope* x64ope);

#endif
