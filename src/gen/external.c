#include "./external.h"
#include "./util.h"

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

    Srt* func_decl = vector_at(srt->children, 0);
    codegen->_srt = func_decl;

    Srt* func_body = vector_at(srt->children, 1);
    codegen->_table = new_symboltable();

    Vector* func_codes = new_vector();
    int num_children = vector_size(func_body->children);
    for (int i = 0; i < num_children; i++) {
        codegen->_srt = vector_at(func_body->children, i);
        Vector* sub_codes = codegen_generate_code(codegen);
        vector_extend(func_codes, sub_codes);
        delete_vector(sub_codes, free);
    }

    append_code(codes, "    .global _%s\n", func_decl->ident_name);
    append_code(codes, "_%s:\n", func_decl->ident_name);
    append_code(codes, "    pushq  %%rbp\n");
    append_code(codes, "    movq  %%rsp, %%rbp\n");
    append_code(codes, "    subq  $%d, %%rsp\n", codegen->_table->_memory_offset);

    vector_extend(codes, func_codes);
    delete_vector(func_codes, free);

    append_code(codes, "    addq  $%d, %%rsp\n", codegen->_table->_memory_offset);
    append_code(codes, "    popq  %%rbp\n");
    append_code(codes, "    ret\n");

    delete_symboltable(codegen->_table);
    codegen->_table = NULL;
    codegen->_srt = srt;
    return codes;
}