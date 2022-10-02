#include "./external.h"
#include "../common/type.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>
#include <string.h>

Vector* gen_translation_unit_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    append_children_immcode(immcgen, codes);
    return codes;
}

Vector* gen_function_definition_immcode(Immcgen* immcgen) {
    Vector* codes = new_vector(&t_immc);
    Srt* srt = immcgen->srt;

    Srt* declarator_srt = vector_at(srt->children, 0);
    char* symbol_name = new_string(declarator_srt->ident_name);
    DType* symbol_dtype = dtype_copy(declarator_srt->dtype);
    symboltable_define_label(immcgen->symbol_table, symbol_name, symbol_dtype);

    immcgen->next_reg_id = -1;
    immcgen->label_id++;
    immcgen->symbol_table = symboltable_enter_scope(immcgen->symbol_table);
    immcgen->symbol_table->memory_size = 0;
    immcgen->return_label_id = immcgen->label_id;

    Vector* param_codes = new_vector(&t_immc);
    Vector* params = declarator_srt->dtype->function->params;
    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        DParam* dparam = vector_at(params, i);

        char* symbol_name = new_string(dparam->name);
        DType* symbol_dtype = dtype_copy(dparam->dtype);
        Symbol* symbol = symboltable_define_memory(immcgen->symbol_table, symbol_name, symbol_dtype);

        ImmcSuffix suffix = immcsuffix_get(dtype_size(dparam->dtype));
        ImmcOpe* dst = new_mem_immcope(symbol->memory_offset);
        ImmcOpe* src = new_arg_immcope(suffix, i);
        vector_push(param_codes, new_inst_immc(IMMC_INST_LDARG, dst, src, NULL));
    }

    immcgen->srt = vector_at(srt->children, 1);
    Vector* body_codes = new_vector(&t_immc);
    append_children_immcode(immcgen, body_codes);

    char* label_name = new_string(declarator_srt->ident_name);
    vector_push(codes, new_label_immc(IMMC_LABEL_FUNCTION, IMMC_VIS_GLOBAL, label_name));

    IntegerLiteral* iliteral = new_signed_iliteral(INTEGER_INT, immcgen->symbol_table->memory_size);
    ImmcOpe* memory_size = new_int_immcope(IMMC_SUFFIX_QUAD, iliteral);
    vector_push(codes, new_inst_immc(IMMC_INST_ENTER, NULL, memory_size, NULL));

    vector_extend(codes, param_codes);
    delete_vector(param_codes);

    vector_extend(codes, body_codes);
    delete_vector(body_codes);

    char* return_label_name = create_label_name(immcgen->return_label_id);
    vector_push(codes, new_label_immc(IMMC_LABEL_NORMAL, IMMC_VIS_NONE, return_label_name));
    vector_push(codes, new_inst_immc(IMMC_INST_LEAVE, NULL, immcope_copy(memory_size), NULL));

    immcgen->symbol_table->memory_size = 0;
    immcgen->symbol_table = symboltable_exit_scope(immcgen->symbol_table);
    immcgen->srt = srt;

    return codes;
}
