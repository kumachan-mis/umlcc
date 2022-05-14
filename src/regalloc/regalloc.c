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
    Vector* control_flow_graph = connect_basic_blocks(basic_blocks);
    control_flow_graph = analyze_liveness(control_flow_graph);
    return control_flow_graph;
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

Vector* analyze_liveness(Vector* control_flow_graph) {
    int blocks_len = vector_size(control_flow_graph);
    while (1) {
        int terminated = 1;
        for (int block_id = blocks_len - 1; block_id >= 0; block_id--) {
            BasicBlock* basic_block = vector_at(control_flow_graph, block_id);
            Set* input = NULL;
            Set* output = new_set(&t_hashable_integer);

            for (SetIter* iter = set_iter_begin(basic_block->succ);
                 !set_iter_end(iter, basic_block->succ);
                 iter = set_iter_next(iter, basic_block->succ)) {
                int* succ_block_id_ref = set_iter_item(iter, basic_block->succ);
                BasicBlock* succ_block = vector_at(control_flow_graph, *succ_block_id_ref);
                Set* prev_output = output;
                output = set_union(prev_output, succ_block->input);
                delete_set(prev_output);
            }
            input = set_copy(output);

            int immcs_len = vector_size(basic_block->immcs);
            for (int i = immcs_len - 1; i >= 0; i--) {
                Immc* immc = vector_at(basic_block->immcs, i);
                if (immc->type != IMMC_INST) continue;
                ImmcOpe* fst_src = immc->inst->fst_src;
                if (fst_src != NULL &&
                    (fst_src->type == OPERAND_REG || fst_src->type == OPERAND_PTR)) {
                    set_add(input, new_integer(fst_src->reg_id));
                }
                ImmcOpe* snd_src = immc->inst->snd_src;
                if (snd_src != NULL &&
                    (snd_src->type == OPERAND_REG || snd_src->type == OPERAND_PTR)) {
                    set_add(input, new_integer(snd_src->reg_id));
                }
                ImmcOpe* dest = immc->inst->dest;
                if (dest != NULL) {
                    if (dest->type == OPERAND_REG)
                        set_remove(input, &dest->reg_id);
                    else if (dest->type == OPERAND_PTR)
                        set_add(input, new_integer(dest->reg_id));
                }
            }

            if (!set_equals(input, basic_block->input) ||
                !set_equals(output, basic_block->output)) {
                delete_set(basic_block->input);
                delete_set(basic_block->output);
                basic_block->input = input;
                basic_block->output = output;
                terminated = 0;
            }
        }
        if (terminated) break;
    }

    return control_flow_graph;
}
