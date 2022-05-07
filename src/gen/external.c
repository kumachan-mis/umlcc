#include "./external.h"
#include "../common/common.h"
#include "../immc/immc.h"
#include "./util.h"

#include <stdlib.h>
#include <string.h>

Vector* gen_translation_unit_code(Codegen* codegen) {
    Vector* codes = new_vector();
    append_children_code(codegen, codes);
    return codes;
}

Vector* gen_function_definition_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    Srt* declarator_srt = vector_at(srt->children, 0);
    char* table_ident_name = string_copy(declarator_srt->ident_name);
    CType* table_ctype = ctype_copy(declarator_srt->ctype);
    symboltable_define(codegen->_global_table, table_ident_name, table_ctype);

    codegen->_local_table = new_symboltable();

    int return_label_len = strlen(declarator_srt->ident_name) + 11;
    codegen->return_label = malloc(return_label_len * sizeof(char));
    memset(codegen->return_label, 0, return_label_len);
    strcat(codegen->return_label, declarator_srt->ident_name);
    strcat(codegen->return_label, "_return");

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
    Vector* body_codes = new_vector();
    append_children_code(codegen, body_codes);

    char* label_name = string_copy(declarator_srt->ident_name);
    vector_push(codes, new_label_immc(LABEL_FUNCTION, VISIBILITY_GLOBAL, label_name));
    ImmcOpe* memory_size = new_imm_immcope(codegen->_local_table->_memory_size);
    vector_push(codes, new_inst_immc(INST_ENTER, NULL, memory_size, NULL));

    vector_extend(codes, param_codes);
    delete_vector(param_codes, free);

    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);

    char* return_label_name = string_copy(codegen->return_label);
    vector_push(codes, new_label_immc(LABEL_NORMAL, VISIBILITY_DEFAULT, return_label_name));
    vector_push(codes, new_inst_immc(INST_LEAVE, NULL, immcope_copy(memory_size), NULL));

    free(codegen->return_label);
    codegen->return_label = NULL;
    delete_symboltable(codegen->_local_table);
    codegen->_local_table = NULL;
    codegen->_srt = srt;

    return codes;
}
