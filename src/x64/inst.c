#include "./inst.h"
#include "../immc/immc.h"
#include "./util.h"

Vector* gen_load_x64code(X64gen* x64gen);
Vector* gen_addr_x64code(X64gen* x64gen);
Vector* gen_store_x64code(X64gen* x64gen);
Vector* gen_ldarg_x64code(X64gen* x64gen);
Vector* gen_starg_x64code(X64gen* x64gen);
Vector* gen_add_x64code(X64gen* x64gen);
Vector* gen_sub_x64code(X64gen* x64gen);
Vector* gen_mul_x64code(X64gen* x64gen);
Vector* gen_div_x64code(X64gen* x64gen);
Vector* gen_mod_x64code(X64gen* x64gen);
Vector* gen_call_x64code(X64gen* x64gen);
Vector* gen_enter_x64code(X64gen* x64gen);
Vector* gen_leave_x64code(X64gen* x64gen);

Vector* gen_inst_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->_immcs, x64gen->index);
    switch (immc->inst->type) {
        case INST_LOAD:
            return gen_load_x64code(x64gen);
        case INST_ADDR:
            return gen_addr_x64code(x64gen);
        case INST_STORE:
            return gen_store_x64code(x64gen);
        case INST_LDARG:
            return gen_ldarg_x64code(x64gen);
        case INST_STARG:
            return gen_starg_x64code(x64gen);
        case INST_ADD:
            return gen_add_x64code(x64gen);
        case INST_SUB:
            return gen_sub_x64code(x64gen);
        case INST_MUL:
            return gen_mul_x64code(x64gen);
        case INST_DIV:
            return gen_div_x64code(x64gen);
        case INST_MOD:
            return gen_mod_x64code(x64gen);
        case INST_CALL:
            return gen_call_x64code(x64gen);
        case INST_ENTER:
            return gen_enter_x64code(x64gen);
        case INST_LEAVE:
            return gen_leave_x64code(x64gen);
    }
}

Vector* gen_load_x64code(X64gen* x64gen) {
}

Vector* gen_addr_x64code(X64gen* x64gen) {
}

Vector* gen_store_x64code(X64gen* x64gen) {
}

Vector* gen_ldarg_x64code(X64gen* x64gen) {
}

Vector* gen_starg_x64code(X64gen* x64gen) {
}

Vector* gen_add_x64code(X64gen* x64gen) {
}

Vector* gen_sub_x64code(X64gen* x64gen) {
}

Vector* gen_mul_x64code(X64gen* x64gen) {
}

Vector* gen_div_x64code(X64gen* x64gen) {
}

Vector* gen_mod_x64code(X64gen* x64gen) {
}

Vector* gen_call_x64code(X64gen* x64gen) {
}

Vector* gen_enter_x64code(X64gen* x64gen) {
}

Vector* gen_leave_x64code(X64gen* x64gen) {
}
