#include "./regalloc.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "../map/map.h"
#include "./basicblock.h"
#include "./liveness.h"

#include <stdlib.h>

Vector* dequeue_external_immcs(RegAlloc* regalloc);

Vector* create_control_flow_graph(Vector* immcs);
Vector* create_basic_blocks(Vector* immcs);
Vector* connect_basic_blocks(Vector* basic_blocks);
Vector* analyze_register_flow(Vector* basic_blocks);
int update_register_flow(Vector* control_flow_graph, BasicBlock* basic_block);

Vector* analyze_register_liveness(Vector* control_flow_graph);
void update_register_liveness(Vector* livenesses, BasicBlock* basic_block, int block_offset);

Vector* determine_allocation(Vector* livenesses, int num_real_regs);
void free_unused_register(Vector* statuses, Vector* livenesses, int virtual_reg_id);
int allocate_real_register(Vector* allocations, Vector* statuses, int virtual_reg_id);
void spil_register(Vector* allocations, Vector* statuses, Vector* livenesses, int virtual_reg_id);

void convert_allocated_immcs(Vector* immcs, Vector* allocations);

RegAlloc* new_regalloc(Vector* immcs, int num_real_regs) {
    RegAlloc* regalloc = malloc(sizeof(RegAlloc));
    regalloc->_immcs = immcs;
    regalloc->_index = 0;
    regalloc->_num_real_regs = num_real_regs;
    return regalloc;
}

void delete_regalloc(RegAlloc* regalloc) {
    delete_vector(regalloc->_immcs);
    free(regalloc);
}

Vector* regalloc_allocate_regs(RegAlloc* regalloc) {
    Vector* allocated_immcs = new_vector(&t_immc);

    int immcs_len = vector_size(regalloc->_immcs);
    while (regalloc->_index < immcs_len) {
        Vector* immcs = dequeue_external_immcs(regalloc);

        Vector* control_flow_graph = create_control_flow_graph(immcs);
        Vector* livenesses = analyze_register_liveness(control_flow_graph);
        delete_vector(control_flow_graph);

        Vector* allocations = determine_allocation(livenesses, regalloc->_num_real_regs);
        delete_vector(livenesses);

        convert_allocated_immcs(immcs, allocations);
        delete_vector(allocations);

        vector_extend(allocated_immcs, immcs);
        delete_vector(immcs);
    }

    return allocated_immcs;
}

Vector* dequeue_external_immcs(RegAlloc* regalloc) {
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

Vector* create_control_flow_graph(Vector* immcs) {
    Vector* basic_blocks = create_basic_blocks(immcs);
    Vector* control_flow_graph = connect_basic_blocks(basic_blocks);
    control_flow_graph = analyze_register_flow(control_flow_graph);
    return control_flow_graph;
}

Vector* create_basic_blocks(Vector* immcs) {
    Vector* basic_blocks = new_vector(&t_basicblock);

    Vector* block_immcs = new_vector(&t_immc);
    Vector* next_block_immcs = NULL;

    int sequence_len = vector_size(immcs);
    int i = 0;
    while (i < sequence_len) {
        while (i < sequence_len) {
            Immc* immc = vector_at(immcs, i);
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

    if (block_immcs != NULL) delete_vector(block_immcs);
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

Vector* analyze_register_flow(Vector* control_flow_graph) {
    int blocks_len = vector_size(control_flow_graph);
    int terminated = 0;
    while (!terminated) {
        terminated = 1;
        for (int block_id = blocks_len - 1; block_id >= 0; block_id--) {
            BasicBlock* basic_block = vector_at(control_flow_graph, block_id);
            int converged = update_register_flow(control_flow_graph, basic_block);
            terminated = terminated && converged;
        }
    }
    return control_flow_graph;
}

int update_register_flow(Vector* control_flow_graph, BasicBlock* basic_block) {
    Set* input = NULL;
    Set* output = new_set(&t_hashable_integer);

    for (SetIter* iter = set_iter_begin(basic_block->succ); !set_iter_end(iter, basic_block->succ);
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
        if (fst_src != NULL && (fst_src->type == OPERAND_REG || fst_src->type == OPERAND_PTR)) {
            set_add(input, new_integer(fst_src->reg_id));
        }

        ImmcOpe* snd_src = immc->inst->snd_src;
        if (snd_src != NULL && (snd_src->type == OPERAND_REG || snd_src->type == OPERAND_PTR)) {
            set_add(input, new_integer(snd_src->reg_id));
        }

        ImmcOpe* dest = immc->inst->dest;
        if (dest != NULL && dest->type == OPERAND_REG) {
            set_remove(input, &dest->reg_id);
        } else if (dest != NULL && dest->type == OPERAND_PTR) {
            set_add(input, new_integer(dest->reg_id));
        }
    }

    int input_converged = set_equals(input, basic_block->input);
    int output_converged = set_equals(output, basic_block->output);

    delete_set(basic_block->input);
    delete_set(basic_block->output);

    basic_block->input = input;
    basic_block->output = output;

    return input_converged && output_converged;
}

Vector* analyze_register_liveness(Vector* control_flow_graph) {
    Vector* register_liveness = new_vector(&t_regliveness);

    int blocks_len = vector_size(control_flow_graph);
    int block_offset = 0;
    for (int block_id = 0; block_id < blocks_len; block_id++) {
        BasicBlock* basic_block = vector_at(control_flow_graph, block_id);
        update_register_liveness(register_liveness, basic_block, block_offset);
        block_offset += vector_size(basic_block->immcs);
    }

    return register_liveness;
}

void update_register_liveness(Vector* livenesses, BasicBlock* basic_block, int block_offset) {
    int immcs_len = vector_size(basic_block->immcs);
    for (int i = 0; i < immcs_len; i++) {
        Immc* immc = vector_at(basic_block->immcs, i);
        if (immc->type != IMMC_INST) continue;

        ImmcOpe* fst_src = immc->inst->fst_src;
        if (fst_src != NULL && (fst_src->type == OPERAND_REG || fst_src->type == OPERAND_PTR)) {
            RegLiveness* liveness = vector_at(livenesses, fst_src->reg_id);
            liveness->last_use_index = block_offset + i;
        }

        ImmcOpe* snd_src = immc->inst->snd_src;
        if (snd_src != NULL && (snd_src->type == OPERAND_REG || snd_src->type == OPERAND_PTR)) {
            RegLiveness* liveness = vector_at(livenesses, snd_src->reg_id);
            liveness->last_use_index = block_offset + i;
        }

        ImmcOpe* dest = immc->inst->dest;
        if (dest != NULL && dest->type == OPERAND_REG) {
            vector_fill(livenesses, dest->reg_id + 1, new_regliveness());
            RegLiveness* liveness = vector_at(livenesses, dest->reg_id);
            if (regliveness_isinit(liveness)) liveness->first_def_index = block_offset + i;
        } else if (dest != NULL && dest->type == OPERAND_PTR) {
            RegLiveness* liveness = vector_at(livenesses, dest->reg_id);
            liveness->last_use_index = block_offset + i;
        }
    }

    Set* output = basic_block->output;
    for (SetIter* iter = set_iter_begin(output); !set_iter_end(iter, output);
         iter = set_iter_next(iter, output)) {
        int* reg_id_ref = set_iter_item(iter, output);
        RegLiveness* liveness = vector_at(livenesses, *reg_id_ref);
        liveness->last_use_index = block_offset + immcs_len;
    }
}

Vector* determine_allocation(Vector* livenesses, int num_real_regs) {
    int num_virtual_regs = vector_size(livenesses);

    // -1: free, 0~: virtual register using allocatee register
    Vector* statuses = new_vector(&t_integer);
    vector_fill(statuses, num_real_regs, new_integer(-1));

    // -1: memory, 0~: allocatee register used by virtual register
    Vector* allocations = new_vector(&t_integer);
    vector_fill(allocations, num_virtual_regs, new_integer(-1));

    for (int virtual_reg_id = 0; virtual_reg_id < num_virtual_regs; virtual_reg_id++) {
        free_unused_register(statuses, livenesses, virtual_reg_id);
        int success = allocate_real_register(allocations, statuses, virtual_reg_id);
        if (!success) spil_register(allocations, statuses, livenesses, virtual_reg_id);
    }

    delete_vector(statuses);
    return allocations;
}

void free_unused_register(Vector* statuses, Vector* livenesses, int virtual_reg_id) {
    RegLiveness* liveness = vector_at(livenesses, virtual_reg_id);
    int num_real_regs = vector_size(statuses);

    for (int real_reg_id = 0; real_reg_id < num_real_regs; real_reg_id++) {
        int* alloc_virtual_reg_id_ref = vector_at(statuses, real_reg_id);
        if (*alloc_virtual_reg_id_ref == -1) continue;

        RegLiveness* alloc_liveness = vector_at(livenesses, *alloc_virtual_reg_id_ref);
        if (alloc_liveness->last_use_index <= liveness->first_def_index) {
            vector_set(statuses, real_reg_id, new_integer(-1));
        }
    }
}

int allocate_real_register(Vector* allocations, Vector* statuses, int virtual_reg_id) {
    int success = 0;
    int num_real_regs = vector_size(statuses);

    for (int real_reg_id = 0; real_reg_id < num_real_regs; real_reg_id++) {
        int* alloc_virtual_reg_id_ref = vector_at(statuses, real_reg_id);
        if (*alloc_virtual_reg_id_ref != -1) continue;

        vector_set(statuses, real_reg_id, new_integer(virtual_reg_id));
        vector_set(allocations, virtual_reg_id, new_integer(real_reg_id));
        success = 1;
        break;
    }

    return success;
}

void spil_register(Vector* allocations, Vector* statuses, Vector* livenesses, int virtual_reg_id) {
    RegLiveness* liveness = vector_at(livenesses, virtual_reg_id);
    int num_real_regs = vector_size(statuses);

    int spilled_virtual_reg_id = virtual_reg_id;
    int spilled_real_reg_id = -1;
    int max_range_len = liveness->last_use_index - liveness->first_def_index;

    for (int real_reg_id = 0; real_reg_id < num_real_regs; real_reg_id++) {
        int* alloc_virtual_reg_id_ref = vector_at(statuses, real_reg_id);
        RegLiveness* alloc_liveness = vector_at(livenesses, *alloc_virtual_reg_id_ref);
        int alloc_range_len = alloc_liveness->last_use_index - alloc_liveness->first_def_index;
        if (max_range_len >= alloc_range_len) continue;

        spilled_real_reg_id = real_reg_id;
        spilled_virtual_reg_id = *alloc_virtual_reg_id_ref;
        max_range_len = alloc_range_len;
    }

    if (spilled_virtual_reg_id == virtual_reg_id) {
        vector_set(allocations, virtual_reg_id, new_integer(-1));
        return;
    }

    vector_set(allocations, spilled_virtual_reg_id, new_integer(-1));
    vector_set(allocations, virtual_reg_id, new_integer(spilled_real_reg_id));
    vector_set(statuses, spilled_real_reg_id, new_integer(virtual_reg_id));
}

void convert_allocated_immcs(Vector* immcs, Vector* allocations) {
    int immcs_len = vector_size(immcs);
    for (int i = 0; i < immcs_len; i++) {
        Immc* immc = vector_at(immcs, i);
        if (immc->type != IMMC_INST) continue;

        ImmcOpe* fst_src = immc->inst->fst_src;
        if (fst_src != NULL && (fst_src->type == OPERAND_REG || fst_src->type == OPERAND_PTR)) {
            int* allocation = vector_at(allocations, fst_src->reg_id);
            fst_src->reg_id = *allocation;
        }

        ImmcOpe* snd_src = immc->inst->snd_src;
        if (snd_src != NULL && (snd_src->type == OPERAND_REG || snd_src->type == OPERAND_PTR)) {
            int* allocation = vector_at(allocations, snd_src->reg_id);
            snd_src->reg_id = *allocation;
        }

        ImmcOpe* dest = immc->inst->dest;
        if (dest != NULL && (dest->type == OPERAND_REG || dest->type == OPERAND_PTR)) {
            int* allocation = vector_at(allocations, dest->reg_id);
            dest->reg_id = *allocation;
        }
    }
}
