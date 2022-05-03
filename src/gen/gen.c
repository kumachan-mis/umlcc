#include "./gen.h"
#include "./declaration.h"
#include "./expression.h"
#include "./external.h"
#include "./statement.h"

#include <stdio.h>
#include <stdlib.h>

Codegen* new_codegen(Srt* srt) {
    Codegen* codegen = malloc(sizeof(Codegen));
    codegen->_srt = srt;
    codegen->register_name = 0;
    codegen->_global_table = new_symboltable();
    codegen->_local_table = NULL;
    return codegen;
}

void delete_codegen(Codegen* codegen) {
    delete_srt(codegen->_srt);
    delete_symboltable(codegen->_global_table);
    if (codegen->_local_table != NULL) delete_symboltable(codegen->_local_table);
    free(codegen);
}

Vector* codegen_generate_code(Codegen* codegen) {
    Vector* codes = NULL;
    Srt* srt = codegen->_srt;

    switch (srt->type) {
        case SRT_TRAS_UNIT:
            codes = gen_translation_unit_code(codegen);
            break;
        case SRT_FUNC_DEF:
            codes = gen_function_definition_code(codegen);
            break;
        case SRT_DECL_LIST:
            codes = gen_decl_list_code(codegen);
            break;
        case SRT_INIT_DECL:
            codes = gen_init_decl_code(codegen);
            break;
        case SRT_DECL:
            codes = gen_decl_code(codegen);
            break;
        case SRT_CMPD_STMT:
            codegen->_local_table = symboltable_enter_scope(codegen->_local_table);
            codes = gen_compound_stmt_code(codegen);
            codegen->_local_table = symboltable_exit_scope(codegen->_local_table);
            break;
        case SRT_EXPR_STMT:
            codes = gen_expression_stmt_code(codegen);
            break;
        case SRT_ASSIGN_EXPR:
            codes = gen_assignment_expr_code(codegen);
            break;
        case SRT_ADD_EXPR:
        case SRT_SUB_EXPR:
            codes = gen_additive_expr_code(codegen);
            break;
        case SRT_MUL_EXPR:
        case SRT_DIV_EXPR:
        case SRT_MOD_EXPR:
            codes = gen_multiplicative_expr_code(codegen);
            break;
        case SRT_ADDR_EXPR:
            codes = gen_unary_expr_code(codegen);
            break;
        case SRT_CALL_EXPR:
            codes = gen_postfix_expr_code(codegen);
            break;
        case SRT_IDENT_EXPR:
        case SRT_INT_EXPR:
            codes = gen_primary_expr_code(codegen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
            ;
    }

    return codes;
}
