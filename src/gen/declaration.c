#include "./declaration.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Vector* gen_decl_code(Codegen* codegen) {
    Ast* ast = codegen->_ast;
    int num_children = vector_size(ast->children);
    for (int i = 1; i < num_children; i++) {
        Ast* child = vector_at(ast->children, i);
        Ast* ident = vector_at(child->children, 0);
        if (symboltable_search_symbol(codegen->_table,ident->ident_name) != NULL) {
            fprintf(stderr, "Error: identifier '%s' is already declared\n", ident->ident_name);
            exit(1);
        }

        char* ident_name = malloc((strlen(ident->ident_name) + 1) *sizeof(char));
        strcpy(ident_name, ident->ident_name);
        symboltable_define_symbol(codegen->_table, ident_name);
    }
    return new_vector();
}