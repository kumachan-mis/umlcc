#include "./register.h"

#include <stdlib.h>

ImmcRegister* new_immcreg(int reg_id, int is_unsigned) {
    ImmcRegister* immcreg = malloc(sizeof(ImmcRegister));
    immcreg->reg_id = reg_id;
    immcreg->is_unsigned = is_unsigned;
    return immcreg;
}

ImmcRegister* new_signed_immcreg(int reg_id) {
    ImmcRegister* immcreg = new_immcreg(reg_id, 0);
    return immcreg;
}

ImmcRegister* new_unsigned_immcreg(int reg_id) {
    ImmcRegister* immcreg = new_immcreg(reg_id, 1);
    return immcreg;
}

ImmcRegister* immcreg_copy(ImmcRegister* immcreg) {
    ImmcRegister* copied_immcreg = malloc(sizeof(ImmcRegister));
    copied_immcreg->reg_id = immcreg->reg_id;
    copied_immcreg->is_unsigned = immcreg->is_unsigned;
    return copied_immcreg;
}

void delete_immcreg(ImmcRegister* immcreg) {
    free(immcreg);
}
