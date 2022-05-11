#include "./regalloc.h"
#include "./basicblock.h"
#include "./lifetime.h"
#include "../immc/immc.h"

#include <stdlib.h>

Vector* dequeue_external_sequence(RegAlloc* regalloc);
Vector* create_basic_blocks(Vector* external_sequence);
Vector* analyze_reg_lifetimes(Vector* basic_blocks);
Vector* determine_allocation(Vector* reg_lifetimes);
Vector* gen_allocated_immcs(RegAlloc* regalloc, Vector* allocation);

RegAlloc* new_regalloc(Vector* immcs, int num_physical_regs) {
    RegAlloc* regalloc = malloc(sizeof(RegAlloc));
    regalloc->_immcs = immcs;
    regalloc->_index = 0;
    regalloc->_num_physical_regs = num_physical_regs;
    return regalloc;
}

void delete_regalloc(RegAlloc* regalloc) {
    delete_vector(regalloc->_immcs, delete_immc);
    free(regalloc);
}

Vector* regalloc_allocate_physical_regs(RegAlloc* regalloc) {
    Vector* immcs = new_vector();

    int immcs_len = vector_size(regalloc->_immcs);
    while (regalloc->_index < immcs_len) {
        Vector* external_sequence = dequeue_global_section(regalloc);

        Vector* basic_blocks = create_basic_blocks(external_sequence);
        delete_vector(external_sequence, (void (*)(void* item))delete_immc);

        Vector* regs_lifetime = analyze_regs_lifetime(basic_blocks);
        delete_vector(basic_blocks, (void (*)(void* item))delete_basicblock);

        Vector* allocation = determine_allocation(regs_lifetime);
        delete_vector(regs_lifetime, (void (*)(void* item))delete_lifetime);

        Vector* sub_immcs = gen_allocated_immcs(regalloc, allocation);
        vector_extend(immcs, sub_immcs);
        delete_vector(sub_immcs, (void (*)(void* item))delete_immc);
    }

    return immcs;
}

Vector* dequeue_external_sequence(RegAlloc* regalloc) {
    Vector* sequence = new_vector();

    Immc* label = vector_at(regalloc->_immcs, regalloc->_index);
    regalloc->_index++;
    vector_push(sequence, immc_copy(label));

    int immcs_len = vector_size(regalloc->_immcs);
    while (regalloc->_index < immcs_len) {
        Immc* immc = vector_at(regalloc->_immcs, regalloc->_index);
        if (immc->type == IMMC_LABEL && immc->label->type == LABEL_FUNCTION) break;
        regalloc->_index++;
        vector_push(sequence, immc_copy(immc));
    }

    return sequence;
}

Vector* create_basic_blocks(Vector* external_sequence) {

}

Vector* analyze_regs_lifetime(Vector* basic_blocks) {

}

Vector* determine_allocation(Vector* regs_lifetime) {

}

Vector* gen_allocated_immcs(RegAlloc* regalloc, Vector* allocation) {

}
