#include "./external.h"
#include "../common/common.h"
#include "./util.h"

#include <stdlib.h>

// If the class is INTEGER, the next available register of the sequence
// %rdi, %rsi, %rdx, %rcx, %r8 and %r9 is used.
// cf. System V Application Binary Interface (p20)
//     https://uclibc.org/docs/psABI-x86_64.pdf
char param_regs[][6] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

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
        if (i < 6) {
            append_code(param_codes, "    movl  %s, -%d(%%rbp)\n", param_regs[i],
                        symbol->memory_offset);
        } else {
            // (1-indexed non-register param no.) * (bytes of memory address) +
            // (offset for pushq %%rbp)
            int param_offset = (i - 5) * 8 + 8;
            append_code(param_codes, "    movl  %d(%%rbp), %%eax\n", param_offset);
            append_code(param_codes, "    movl  %%eax, -%d(%%rbp)\n", symbol->memory_offset);
        }
    }

    codegen->_srt = vector_at(srt->children, 1);
    Vector* body_codes = gen_children_code(codegen);

    // The end of the input argument area shall be aligned on a 16 byte boundary.
    // (It is efficient to keep 16-bytes-boundary alignment in advance)
    int aligned_memory_offset = ((codegen->_local_table->_memory_offset + 15) / 16) * 16;

    append_code(codes, "    .globl %s\n", table_ident_name);
    append_code(codes, "%s:\n", table_ident_name);
    append_code(codes, "    pushq  %%rbp\n");
    append_code(codes, "    movq  %%rsp, %%rbp\n");
    append_code(codes, "    subq  $%d, %%rsp\n", aligned_memory_offset);

    vector_extend(codes, param_codes);
    delete_vector(param_codes, free);

    vector_extend(codes, body_codes);
    delete_vector(body_codes, free);

    append_code(codes, "    addq  $%d, %%rsp\n", aligned_memory_offset);
    append_code(codes, "    popq  %%rbp\n");
    append_code(codes, "    ret\n");

    delete_symboltable(codegen->_local_table);
    codegen->_local_table = NULL;
    codegen->_srt = srt;

    return codes;
}
