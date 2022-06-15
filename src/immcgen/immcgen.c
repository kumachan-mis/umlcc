#include "./immcgen.h"
#include "./declaration.h"
#include "./expression.h"
#include "./external.h"
#include "./statement.h"

#include <stdio.h>
#include <stdlib.h>

Immcgen* new_immcgen(Srt* srt) {
    Immcgen* immcgen = malloc(sizeof(Immcgen));
    immcgen->srt = srt;
    immcgen->global_table = new_symboltable();
    immcgen->local_table = NULL;
    immcgen->virtual_reg_suffix = IMMC_SUFFIX_NONE;
    immcgen->virtual_reg_id = -1;
    immcgen->initialized_dtype = NULL;
    immcgen->initialized_offset = -1;
    immcgen->return_label_id = -1;
    immcgen->label_id = -1;
    return immcgen;
}

Vector* immcgen_generate_immcode(Immcgen* immcgen) {
    Vector* codes = NULL;
    Srt* srt = immcgen->srt;

    switch (srt->type) {
        case SRT_TRAS_UNIT:
            codes = gen_translation_unit_immcode(immcgen);
            break;
        case SRT_FUNC_DEF:
            codes = gen_function_definition_immcode(immcgen);
            break;
        case SRT_DECL_LIST:
            codes = gen_decl_list_immcode(immcgen);
            break;
        case SRT_INIT_DECL:
            codes = gen_init_decl_immcode(immcgen);
            break;
        case SRT_DECL:
            codes = gen_decl_immcode(immcgen);
            break;
        case SRT_INIT:
            codes = gen_initializer_immcode(immcgen);
            break;
        case SRT_CMPD_STMT:
            immcgen->local_table = symboltable_enter_scope(immcgen->local_table);
            codes = gen_compound_stmt_immcode(immcgen);
            immcgen->local_table = symboltable_exit_scope(immcgen->local_table);
            break;
        case SRT_RET_STMT:
            codes = gen_return_stmt_immcode(immcgen);
            break;
        case SRT_EXPR_STMT:
            codes = gen_expression_stmt_immcode(immcgen);
            break;
        case SRT_ASSIGN_EXPR:
            codes = gen_assignment_expr_immcode(immcgen);
            break;
        case SRT_LOR_EXPR:
            codes = gen_logical_or_expr_immcode(immcgen);
            break;
        case SRT_LAND_EXPR:
            codes = gen_logical_and_expr_immcode(immcgen);
            break;
        case SRT_EQUAL_EXPR:
        case SRT_NEQUAL_EXPR:
            codes = gen_equality_expr_immcode(immcgen);
            break;
        case SRT_ADD_EXPR:
        case SRT_SUB_EXPR:
            codes = gen_additive_expr_immcode(immcgen);
            break;
        case SRT_PADD_EXPR:
        case SRT_PSUB_EXPR:
        case SRT_PDIFF_EXPR:
            codes = gen_pointer_additive_expr_immcode(immcgen);
            break;
        case SRT_MUL_EXPR:
        case SRT_DIV_EXPR:
        case SRT_MOD_EXPR:
            codes = gen_multiplicative_expr_immcode(immcgen);
            break;
        case SRT_ADDR_EXPR:
        case SRT_INDIR_EXPR:
        case SRT_LNOT_EXPR:
            codes = gen_unary_expr_immcode(immcgen);
            break;
        case SRT_CALL_EXPR:
            codes = gen_postfix_expr_immcode(immcgen);
            break;
        case SRT_IDENT_EXPR:
        case SRT_INT_EXPR:
        case SRT_CHAR_EXPR:
            codes = gen_primary_expr_immcode(immcgen);
            break;
        default:
            fprintf(stderr, "Error: unexpected srt type %d\n", srt->type);
            exit(1);
    }

    return codes;
}

void delete_immcgen(Immcgen* immcgen) {
    delete_srt(immcgen->srt);
    delete_symboltable(immcgen->global_table);
    if (immcgen->local_table != NULL) delete_symboltable(immcgen->local_table);
    if (immcgen->initialized_dtype != NULL) delete_dtype(immcgen->initialized_dtype);
    free(immcgen);
}
