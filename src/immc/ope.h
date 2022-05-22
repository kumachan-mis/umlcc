#ifndef _UMLCC_IMMC_OPE_H_
#define _UMLCC_IMMC_OPE_H_

typedef enum _ImmcOpeType {
    OPERAND_IMM,
    OPERAND_REG,
    OPERAND_PTR,
    OPERAND_MEM,
    OPERAND_LABEL
} ImmcOpeType;

typedef enum _ImmcOpeSuffix {
    SUFFIX_NONE,
    SUFFIX_BYTE,
    SUFFIX_WORD,
    SUFFIX_LONG,
    SUFFIX_QUAD
} ImmcOpeSuffix;

typedef struct _ImmcOpe {
    ImmcOpeType type;
    ImmcOpeSuffix suffix;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

ImmcOpe* new_imm_immcope(int imm_value);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_reg_immcope(ImmcOpeSuffix suffix, int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
void delete_immope(ImmcOpe* immcope);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
ImmcOpeSuffix immcope_suffix_get(int size);
int immcope_suffix_tosize(ImmcOpeSuffix suffix);

#endif
