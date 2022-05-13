#include "./regalloc.h"
#include "./basicblock.h"
#include "./lifetime.h"
#include "../immc/immc.h"
#include "../map/map.h"
#include "../common/common.h"

#include <stdlib.h>

Vector* dequeue_external_sequence(RegAlloc* regalloc);
Vector* create_basic_blocks(Vector* external_sequence);
Vector* analyze_reg_lifetimes(Vector* basic_blocks);
Vector* determine_allocation(Vector* reg_lifetimes);
Vector* gen_allocated_immcs(RegAlloc* regalloc, Vector* allocation);

Vector* split_sequence_into_blocks(Vector* external_sequence);
Vector* resolve_connection_of_blocks(Vector* basic_blocks);
Vector* resolve_regdeps_of_blocks(Vector* basic_blocks);

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
    Vector* basic_blocks = split_sequence_into_blocks(external_sequence);
    basic_blocks = resolve_connection_of_blocks(basic_blocks);
    basic_blocks = resolve_regdeps_of_blocks(basic_blocks);
    return basic_blocks;
}

Vector* analyze_regs_lifetime(Vector* basic_blocks) {

}

Vector* determine_allocation(Vector* regs_lifetime) {

}

Vector* gen_allocated_immcs(RegAlloc* regalloc, Vector* allocation) {

}

Vector* split_sequence_into_blocks(Vector* external_sequence) {
    Vector* basic_blocks = new_vector();
    
    Vector* block_immcs = new_vector();
    Vector* next_block_immcs = NULL;

    int sequence_len = vector_size(external_sequence);
    int i = 0;
    while (i < sequence_len) {
        while (i < sequence_len) {
            Immc* immc = vector_at(external_sequence, i);
            if (immc->type == IMMC_INST && immcinst_isjump(immc->inst)) {
                vector_push(block_immcs, immc_copy(immc));
                i++;
                next_block_immcs = new_vector();
                break;
            }
            if (immc->type == IMMC_LABEL) {
                next_block_immcs = new_vector();
                vector_push(next_block_immcs, immc_copy(immc));
                i++;
                break;
            }
            vector_push(block_immcs, immc_copy(immc));
            i++;
        }

        if (vector_size(block_immcs) == 0) {
            delete_vector(block_immcs, (void (*)(void* item))delete_immc);
        } else {
            vector_push(basic_blocks, new_basicblock(block_immcs));
        }
        block_immcs = next_block_immcs;
        next_block_immcs = NULL;
    }
    
    delete_vector(block_immcs, (void (*)(void* item))delete_immc);
    block_immcs = NULL;

    return basic_blocks;
}

Vector* resolve_connection_of_blocks(Vector* basic_blocks) {
    Map* label_block_map = new_map();

    int blocks_len = vector_size(basic_blocks);
    for (int block_id = 0; block_id < blocks_len; block_id++) {
        BasicBlock* basic_block = vector_at(basic_blocks, block_id);
        Immc* head_immc = vector_at(basic_block->immcs, 0);
        if (head_immc->type == IMMC_LABEL) {
            int* block_id_ref = new_integer(block_id);
            map_set(label_block_map, head_immc->label->label_name, block_id_ref, free);
        }
    }

    for (int block_id = 0; block_id < blocks_len; block_id++) {
        BasicBlock* basic_block = vector_at(basic_blocks, block_id);
        Immc* tail_immc = vector_at(basic_block->immcs, vector_size(basic_block->immcs) - 1);

        if (tail_immc->type == IMMC_INST && immcinst_isjump(tail_immc->inst)) {
            int* jump_dest_block_id_ref = map_get(label_block_map, tail_immc->inst->dest->label_name);
            BasicBlock* dest_basic_block = vector_at(basic_blocks, *jump_dest_block_id_ref);
            basicblock_add_prev_block_id(dest_basic_block, block_id);
        }
        if (block_id < blocks_len - 1 && (tail_immc->type != IMMC_INST || tail_immc->inst->type != INST_JMP)) {
            BasicBlock* dest_basic_block = vector_at(basic_blocks, block_id + 1);
            basicblock_add_prev_block_id(dest_basic_block, block_id);
        }
    }

    delete_map(label_block_map, free);

    return basic_blocks;
}

Vector* resolve_regdeps_of_blocks(Vector* basic_blocks) {

}
