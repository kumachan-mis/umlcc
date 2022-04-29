#include "./external.h"
#include "./util.h"
#include "../common/common.h"

#include <stdlib.h>


Vector* gen_function_params_code(Codegen* codegen);


Vector* gen_translation_unit_code(Codegen* codegen) {
    return gen_children_code(codegen);
}

Vector* gen_function_definition_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    codegen->_srt = vector_at(srt->children, 0);
    char* table_ident_name = string_copy(codegen->_srt->ident_name);
    CType* table_ctype = ctype_copy(codegen->_srt->ctype);
    symboltable_define(codegen->_global_table, table_ident_name, table_ctype);

    codegen->_local_table = new_symboltable();
    Vector* param_codes = gen_function_params_code(codegen);

    codegen->_srt = vector_at(srt->children, 1);
    Vector* body_codes = gen_children_code(codegen);

    delete_symboltable(codegen->_local_table);
    codegen->_local_table = NULL;

    append_code(codes, "    .global _%s\n", table_ident_name);
    append_code(codes, "_%s:\n", table_ident_name);
    append_code(codes, "    pushq  %%rbp\n");
    append_code(codes, "    movq  %%rsp, %%rbp\n");
    append_code(codes, "    subq  $%d, %%rsp\n", codegen->_local_table->_memory_offset);

    vector_extend(codes, param_codes);
    delete_vector(param_codes, free);

    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);

    append_code(codes, "    addq  $%d, %%rsp\n", codegen->_local_table->_memory_offset);
    append_code(codes, "    popq  %%rbp\n");
    append_code(codes, "    ret\n");

    codegen->_srt = srt;
    return codes;
}

Vector* gen_function_params_code(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    char* param_regs[6] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
    Vector* params = srt->ctype->function->params;

    int num_params = vector_size(params);
    for (int i = 0; i < num_params; i++) {
        CParam* cparam = vector_at(params, i);
        char* table_ident_name = string_copy(cparam->ident_name);
        CType* table_ctype = ctype_copy(cparam->ctype);
        Symbol* symbol = symboltable_define(codegen->_local_table, table_ident_name, table_ctype);

        if (i < 6) {
            append_code(codes, "    movq  %s, -%d(%%rbp)\n", param_regs[i], symbol->memory_offset);
        } else {
            int param_offset = (num_params - i) * 8 + 8; // 8 is size of address. + 8 is for "pushq %rbp"
            append_code(codes, "    movq  %d(%%rbp), -%d(%%rbp)\n", param_offset, symbol->memory_offset);
        }
    }

    codegen->_srt = srt;
    return codes;
}
