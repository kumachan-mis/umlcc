#include "./regalloc.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "../map/map.h"
#include "./basicblock.h"

#include <stdlib.h>

Vector* dequeue_external_sequence(RegAlloc* regalloc);
Vector* create_control_flow_graph(Vector* external_sequence);
Vector* determine_allocation(Vector* control_flow_graph);
Vector* gen_allocated_immcs(RegAlloc* regalloc, Vector* allocation);

Vector* create_basic_blocks(Vector* external_sequence);
Vector* connect_basic_blocks(Vector* basic_blocks);
Vector* analyze_liveness(Vector* basic_blocks);

RegAlloc* new_regalloc(Vector* immcs, int num_regs) {
    RegAlloc* regalloc = malloc(sizeof(RegAlloc));
    regalloc->_immcs = immcs;
    regalloc->_index = 0;
    regalloc->_num_regs = num_regs;
    return regalloc;
}

void delete_regalloc(RegAlloc* regalloc) {
    delete_vector(regalloc->_immcs);
    free(regalloc);
}

Vector* regalloc_allocate_regs(RegAlloc* regalloc) {
    Vector* immcs = new_vector(&t_immc);

    int immcs_len = vector_size(regalloc->_immcs);
    while (regalloc->_index < immcs_len) {
        Vector* external_sequence = dequeue_global_section(regalloc);

        Vector* control_flow_graph = create_control_flow_graph(external_sequence);
        delete_vector(external_sequence);

        Vector* allocation = determine_allocation(control_flow_graph);
        delete_vector(control_flow_graph);

        Vector* sub_immcs = gen_allocated_immcs(regalloc, allocation);
        vector_extend(immcs, sub_immcs);
        delete_vector(sub_immcs);
    }

    return immcs;
}

Vector* dequeue_external_sequence(RegAlloc* regalloc) {
    Vector* sequence = new_vector(&t_immc);

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

Vector* create_control_flow_graph(Vector* external_sequence) {
    Vector* basic_blocks = create_basic_blocks(external_sequence);
    basic_blocks = connect_basic_blocks(basic_blocks);
    basic_blocks = analyze_liveness(basic_blocks);
    return basic_blocks;
}

Vector* determine_allocation(Vector* regs_lifetime) {}

Vector* gen_allocated_immcs(RegAlloc* regalloc, Vector* allocation) {}

Vector* create_basic_blocks(Vector* external_sequence) {
    Vector* basic_blocks = new_vector(&t_basicblock);

    Vector* block_immcs = new_vector(&t_immc);
    Vector* next_block_immcs = NULL;

    int sequence_len = vector_size(external_sequence);
    int i = 0;
    while (i < sequence_len) {
        while (i < sequence_len) {
            Immc* immc = vector_at(external_sequence, i);
            if (immc->type == IMMC_INST && immcinst_isjump(immc->inst)) {
                vector_push(block_immcs, immc_copy(immc));
                i++;
                next_block_immcs = new_vector(&t_immc);
                break;
            }
            if (immc->type == IMMC_LABEL) {
                next_block_immcs = new_vector(&t_immc);
                vector_push(next_block_immcs, immc_copy(immc));
                i++;
                break;
            }
            vector_push(block_immcs, immc_copy(immc));
            i++;
        }

        if (vector_size(block_immcs) == 0) {
            delete_vector(block_immcs);
        } else {
            vector_push(basic_blocks, new_basicblock(block_immcs));
        }
        block_immcs = next_block_immcs;
        next_block_immcs = NULL;
    }

    delete_vector(block_immcs);
    block_immcs = NULL;

    return basic_blocks;
}

Vector* connect_basic_blocks(Vector* basic_blocks) {
    Map* label_block_map = new_map(&t_hashable_string, &t_integer);

    int blocks_len = vector_size(basic_blocks);
    for (int block_id = 0; block_id < blocks_len; block_id++) {
        BasicBlock* basic_block = vector_at(basic_blocks, block_id);
        Immc* head_immc = vector_at(basic_block->immcs, 0);
        if (head_immc->type != IMMC_LABEL) continue;

        char* label_name = new_string(head_immc->label->name);
        int* block_id_ref = new_integer(block_id);
        map_add(label_block_map, label_name, block_id_ref);
    }

    for (int block_id = 0; block_id < blocks_len; block_id++) {
        BasicBlock* basic_block = vector_at(basic_blocks, block_id);
        Immc* tail_immc = vector_at(basic_block->immcs, vector_size(basic_block->immcs) - 1);

        if (tail_immc->type == IMMC_INST && immcinst_isjump(tail_immc->inst)) {
            int* block_id_ref = map_get(label_block_map, tail_immc->inst->dest->label_name);
            set_add(basic_block->succ, new_integer(*block_id_ref));
        }
        if (block_id < blocks_len - 1 &&
            (tail_immc->type != IMMC_INST || tail_immc->inst->type != INST_JMP)) {
            set_add(basic_block->succ, new_integer(block_id + 1));
        }
    }

    delete_map(label_block_map);
    return basic_blocks;
}

Vector* analyze_liveness(Vector* basic_blocks) {}
