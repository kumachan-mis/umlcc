#ifndef UMLCC_IMMC_OPE_H
#define UMLCC_IMMC_OPE_H

typedef enum ImmcOpeType {
    OPERAND_IMM,
    OPERAND_REG,
    OPERAND_PTR,
    OPERAND_MEM,
    OPERAND_LABEL
} ImmcOpeType;

typedef struct ImmcOpe {
    ImmcOpeType type;
    int imm_value;
    int reg_id;
    int mem_offset;
    char* label_name;
} ImmcOpe;

ImmcOpe* new_imm_immcope(int imm_value);
ImmcOpe* new_ptr_immcope(int reg_id);
ImmcOpe* new_reg_immcope(int reg_id);
ImmcOpe* new_mem_immcope(int mem_offset);
ImmcOpe* new_label_immcope(char* label_name);
ImmcOpe* immcope_copy(ImmcOpe* immcope);
char* immcope_tostring(ImmcOpe* immcope);
void delete_immope(ImmcOpe* immcope);

#endif
