#include "./regalloc.h"
#include "./consts.h"

#include <stdlib.h>

int* new_regalloc() {
    int* regalloc = malloc(NUM_REGS * sizeof(int));
    for (int i = 0; i < NUM_REGS; i++) regalloc[i] = -1;
    return regalloc;
}

void delete_regalloc(int* regalloc) {
    free(regalloc);
}

int regalloc_allocate_caller_saved(int* regalloc, int virtual_reg_id) {
    for (int i = 0; i < NUM_CALLER_SAVED_REGS; i++) {
        int real_reg_id = CALLER_SAVED_REG_IDS[i];
        if (regalloc[real_reg_id] == -1) {
            regalloc[real_reg_id] = virtual_reg_id;
            return real_reg_id;
        }
    }
    return -1;
}

int regalloc_allocate_callee_saved(int* regalloc, int virtual_reg_id) {
    for (int i = 0; i < NUM_CALLEE_SAVED_REGS; i++) {
        int real_reg_id = CALLEE_SAVED_REG_IDS[i];
        if (regalloc[real_reg_id] == -1) {
            regalloc[real_reg_id] = virtual_reg_id;
            return real_reg_id;
        }
    }
    return -1;
}

int regalloc_force_allocate(int* regalloc, int virtual_reg_id, int real_reg_id) {
    regalloc[real_reg_id] = virtual_reg_id;
    return real_reg_id;
}

int regalloc_change_caller_saved_allocation(int* regalloc, int virtual_reg_id) {
    int current_real_reg_id = -1;
    for (int i = 0; i < NUM_CALLER_SAVED_REGS; i++) {
        int real_reg_id = CALLER_SAVED_REG_IDS[i];
        if (regalloc[real_reg_id] == virtual_reg_id) {
            current_real_reg_id = real_reg_id;
            break;
        }
    }

    for (int i = 0; i < NUM_CALLER_SAVED_REGS; i++) {
        int real_reg_id = CALLER_SAVED_REG_IDS[i];
        if (real_reg_id != current_real_reg_id && regalloc[real_reg_id] == -1) {
            regalloc[real_reg_id] = virtual_reg_id;
            regalloc[current_real_reg_id] = -1;
            return real_reg_id;
        }
    }

    return current_real_reg_id;
}

int regalloc_search(int* regalloc, int virtual_reg_id) {
    for (int real_reg_id = 0; real_reg_id < NUM_REGS; real_reg_id++) {
        if (regalloc[real_reg_id] == virtual_reg_id) return real_reg_id;
    }
    return -1;
}

void regalloc_free(int* regalloc, int virtual_reg_id) {
    for (int real_reg_id = 0; real_reg_id < NUM_REGS; real_reg_id++) {
        if (regalloc[real_reg_id] == virtual_reg_id) {
            regalloc[real_reg_id] = -1;
            break;
        }
    }
}
