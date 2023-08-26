typedef struct ImmcRegister {
    int reg_id;
    int is_unsigned;
} ImmcRegister;

ImmcRegister* new_immcreg(int reg_id, int is_unsigned);
ImmcRegister* new_signed_immcreg(int reg_id);
ImmcRegister* new_unsigned_immcreg(int reg_id);
ImmcRegister* immcreg_copy(ImmcRegister* immcreg);
void delete_immcreg(ImmcRegister* immcreg);
