#include "./external.h"
#include "./util.h"
#include "../common/common.h"

#include <stdlib.h>


Vector* gen_translation_unit(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    int num_children = vector_size(srt->children);
    for (int i = 0; i < num_children; i++) {
        codegen->_srt = vector_at(srt->children, i);
        Vector* sub_codes = codegen_generate_code(codegen);
        vector_extend(codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    codegen->_srt = srt;
    return codes;
}

Vector* gen_function_definition(Codegen* codegen) {
    Vector* codes = new_vector();
    Srt* srt = codegen->_srt;

    Srt* declarator_srt = vector_at(srt->children, 0);
    char* table_ident_name = string_copy(declarator_srt->ident_name);
    CType* table_ctype = ctype_copy(declarator_srt->ctype);
    symboltable_define(codegen->_global_table, table_ident_name, table_ctype);

    Srt* body_srt = vector_at(srt->children, 1);
    codegen->_local_table = new_symboltable();

    Vector* body_codes = new_vector();
    int num_children = vector_size(body_srt->children);
    for (int i = 0; i < num_children; i++) {
        codegen->_srt = vector_at(body_srt->children, i);
        Vector* sub_codes = codegen_generate_code(codegen);
        vector_extend(body_codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    append_code(codes, "    .global _%s\n", declarator_srt->ident_name);
    append_code(codes, "_%s:\n", declarator_srt->ident_name);
    append_code(codes, "    pushq  %%rbp\n");
    append_code(codes, "    movq  %%rsp, %%rbp\n");
    append_code(codes, "    subq  $%d, %%rsp\n", codegen->_local_table->_memory_offset);

    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);

    append_code(codes, "    addq  $%d, %%rsp\n", codegen->_local_table->_memory_offset);
    append_code(codes, "    popq  %%rbp\n");
    append_code(codes, "    ret\n");

    delete_symboltable(codegen->_local_table);
    codegen->_local_table = NULL;
    codegen->_srt = srt;
    return codes;
}