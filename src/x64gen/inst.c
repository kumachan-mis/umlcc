#include "../immc/immc.h"
#include "./arithinst.h"
#include "./bitinst.h"
#include "./ctrlinst.h"
#include "./datainst.h"
#include "./shrotinst.h"

#include <stdio.h>
#include <stdlib.h>

Vector* gen_inst_x64code(X64gen* x64gen) {
    Immc* immc = vector_at(x64gen->immcs, x64gen->index);
    switch (immc->inst->type) {
        case IMMC_INST_LOAD:
            return gen_load_x64code(x64gen);
        case IMMC_INST_ADDR:
            return gen_addr_x64code(x64gen);
        case IMMC_INST_STR:
            return gen_str_x64code(x64gen);
        case IMMC_INST_STORE:
            return gen_store_x64code(x64gen);
        case IMMC_INST_LDARG:
            return gen_ldarg_x64code(x64gen);
        case IMMC_INST_STARG:
            return gen_starg_x64code(x64gen);
        case IMMC_INST_STRET:
            return gen_stret_x64code(x64gen);
        case IMMC_INST_ADD:
            return gen_add_x64code(x64gen);
        case IMMC_INST_SUB:
            return gen_sub_x64code(x64gen);
        case IMMC_INST_MUL:
            return gen_mul_x64code(x64gen);
        case IMMC_INST_DIV:
            return gen_div_x64code(x64gen);
        case IMMC_INST_MOD:
            return gen_mod_x64code(x64gen);
        case IMMC_INST_SAL:
            return gen_sal_x64code(x64gen);
        case IMMC_INST_SAR:
            return gen_sar_x64code(x64gen);
        case IMMC_INST_SETEQ:
            return gen_seteq_x64code(x64gen);
        case IMMC_INST_SETNEQ:
            return gen_setneq_x64code(x64gen);
        case IMMC_INST_JMP:
            return gen_jmp_x64code(x64gen);
        case IMMC_INST_JEQ:
            return gen_jeq_x64code(x64gen);
        case IMMC_INST_JNEQ:
            return gen_jneq_x64code(x64gen);
        case IMMC_INST_CALL:
            return gen_call_x64code(x64gen);
        case IMMC_INST_ENTER:
            return gen_enter_x64code(x64gen);
        case IMMC_INST_LEAVE:
            return gen_leave_x64code(x64gen);
        case IMMC_INST_PREP:
            return gen_prep_x64code(x64gen);
        case IMMC_INST_CLEAN:
            return gen_clean_x64code(x64gen);
        default:
            fprintf(stderr, "Error: unexpected imcc inst %d\n", immc->inst->type);
            exit(1);
    }
}
