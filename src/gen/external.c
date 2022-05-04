#include "./external.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>

Vector* gen_translation_unit_code(Codegen* codegen) {
    return gen_children_code(codegen);
}

Vector* gen_function_definition_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    Srt* declarator_srt = vector_at(srt->children, 0);
    char* table_ident_name = string_copy(declarator_srt->ident_name);
    CType* table_ctype = ctype_copy(declarator_srt->ctype);
    symboltable_define(codegen->_global_table, table_ident_name, table_ctype);

    codegen->_local_table = new_symboltable();

    Vector* param_codes = new_vector();
    Vector* params = declarator_srt->ctype->function->params;
    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        CParam* cparam = vector_at(params, i);
        char* table_ident_name = string_copy(cparam->ident_name);
        CType* table_ctype = ctype_copy(cparam->ctype);

        Symbol* symbol = symboltable_define(codegen->_local_table, table_ident_name, table_ctype);
        ImmcOpe* dest = new_mem_immcope(symbol->memory_offset);
        ImmcOpe* src = new_imm_immcope(i);
        vector_push(param_codes, new_inst_immc(INST_LDARG, dest, src, NULL));
    }

    codegen->_srt = vector_at(srt->children, 1);
    Vector* body_codes = gen_children_code(codegen);

    char* label_name = string_copy(declarator_srt->ident_name);
    vector_push(codes, new_label_immc(LABEL_FUNCTION_HEAD, VISIBILITY_GLOBAL, label_name));
    ImmcOpe* memory_size = new_imm_immcope(codegen->_local_table->_memory_size);
    vector_push(codes, new_inst_immc(INST_ENTER, NULL, memory_size, NULL));

    vector_extend(codes, param_codes);
    delete_vector(param_codes, free);

    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);

    memory_size = new_imm_immcope(codegen->_local_table->_memory_size);
    vector_push(codes, new_inst_immc(INST_LEAVE, NULL, memory_size, NULL));
    label_name = string_copy(declarator_srt->ident_name);
    vector_push(codes, new_label_immc(LABEL_FUNCTION_TAIL, VISIBILITY_DEFAULT, label_name));

    delete_symboltable(codegen->_local_table);
    codegen->_local_table = NULL;
    codegen->_srt = srt;

    return codes;
}
