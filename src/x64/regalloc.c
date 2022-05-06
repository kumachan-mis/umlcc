#include "./regalloc.h"
#include "./consts.h"

#include <stdlib.h>

RegAlloc* new_regalloc() {
    RegAlloc* regalloc = malloc(sizeof(RegAlloc));
    regalloc->_allocation = malloc(NUM_REGS * sizeof(int));
    for (int i = 0; i < NUM_REGS; i++)
        regalloc->_allocation[i] = -1;
    return regalloc;
}

void delete_regalloc(RegAlloc* regalloc) {
    free(regalloc->_allocation);
    free(regalloc);
}

int regalloc_allocate(RegAlloc* regalloc, int virtual_reg_id) {
    for (int i = 0; i < NUM_CALLER_SAVED_REGS; i++) {
        int real_reg_id = CALLER_SAVED_REG_IDS[i];
        if (regalloc->_allocation[real_reg_id] == -1) {
            regalloc->_allocation[real_reg_id] = virtual_reg_id;
            return real_reg_id;
        }
    }
    return -1;
}

int regalloc_force_allocate(RegAlloc* regalloc, int virtual_reg_id, int real_reg_id) {
    regalloc->_allocation[real_reg_id] = virtual_reg_id;
    return real_reg_id;
}

int regalloc_realloc(RegAlloc* regalloc, int virtual_reg_id) {
    int current_real_reg_id = -1;

    for (int i = 0; i < NUM_CALLER_SAVED_REGS; i++) {
        int real_reg_id = CALLER_SAVED_REG_IDS[i];
        if (regalloc->_allocation[real_reg_id] == virtual_reg_id) {
            current_real_reg_id = real_reg_id;
            break;
        }
    }

    for (int i = 0; i < NUM_CALLER_SAVED_REGS; i++) {
        int real_reg_id = CALLER_SAVED_REG_IDS[i];
        if (real_reg_id != current_real_reg_id && regalloc->_allocation[real_reg_id] == -1) {
            regalloc->_allocation[real_reg_id] = virtual_reg_id;
            regalloc->_allocation[current_real_reg_id] = -1;
            return real_reg_id;
        }
    }

    return current_real_reg_id;
}

int regalloc_resolve(RegAlloc* regalloc, int virtual_reg_id) {
    for (int real_reg_id = 0; real_reg_id < NUM_REGS; real_reg_id++) {
        if (regalloc->_allocation[real_reg_id] == virtual_reg_id) return real_reg_id;
    }
    return -1;
}

void regalloc_free(RegAlloc* regalloc, int virtual_reg_id) {
    for (int real_reg_id = 0; real_reg_id < NUM_REGS; real_reg_id++) {
        if (regalloc->_allocation[real_reg_id] == virtual_reg_id) {
            regalloc->_allocation[real_reg_id] = -1;
            break;
        }
    }
}

int regalloc_lock(RegAlloc* regalloc) {
    return regalloc_allocate(regalloc, 0);
}

int regalloc_force_lock(RegAlloc* regalloc, int real_reg_id) {
    return regalloc_force_allocate(regalloc, 0, real_reg_id);
}

int regalloc_usedby(RegAlloc* regalloc, int real_reg_id) {
    return regalloc->_allocation[real_reg_id];
}

void regalloc_unlock(RegAlloc* regalloc, int real_reg_id) {
    regalloc->_allocation[real_reg_id] = -1;
}

Vector* regalloc_evacuate(RegAlloc* regalloc) {
    Vector* evacuation_table = new_vector();

    int callee = 0;
    for (int caller = 0; caller < NUM_CALLER_SAVED_REGS; caller++) {
        int caller_saved_id = CALLER_SAVED_REG_IDS[caller];
        int callee_saved_id = CALLEE_SAVED_REG_IDS[callee];
        if (regalloc->_allocation[caller_saved_id] == -1) continue;

        RegEvacuationEntry* entry = malloc(sizeof(RegEvacuationEntry));
        if (callee < NUM_CALLEE_SAVED_REGS) {
            regalloc->_allocation[callee_saved_id] = regalloc->_allocation[caller_saved_id];
            regalloc->_allocation[caller_saved_id] = -1;
            entry->caller_saved_index = caller;
            entry->callee_saved_index = callee;
        } else {
            entry->caller_saved_index = caller;
            entry->callee_saved_index = -1;
        }

        vector_push(evacuation_table, entry);
        callee++;
    }

    return evacuation_table;
}

void regalloc_restore(RegAlloc* regalloc, Vector* evacuation_table) {
    int evaluation_count = vector_size(evacuation_table);
    for (int i = 0; i < evaluation_count; i++) {
        RegEvacuationEntry* entry = vector_at(evacuation_table, i);
        if (entry->callee_saved_index == -1) continue;

        int caller_saved_id = CALLER_SAVED_REG_IDS[entry->caller_saved_index];
        int callee_saved_id = CALLEE_SAVED_REG_IDS[entry->callee_saved_index];
        regalloc->_allocation[caller_saved_id] = regalloc->_allocation[callee_saved_id];
        regalloc->_allocation[callee_saved_id] = -1;
    }
    delete_vector(evacuation_table, free);
}
