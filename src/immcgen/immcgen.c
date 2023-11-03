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
    immcgen->symbol_table = new_symboltable();
    immcgen->tag_table = new_tagtable();
    immcgen->expr_reg_suffix = IMMC_SUFFIX_NONE;
    immcgen->expr_reg = NULL;
    immcgen->next_reg_id = -1;
    immcgen->initialized_dtype = NULL;
    immcgen->initialized_offset = -1;
    immcgen->continue_label_id = -1;
    immcgen->break_label_id = -1;
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
        case SRT_TAG_DECL:
            codes = gen_tag_decl_immcode(immcgen);
            break;
        case SRT_INIT_DECL:
            codes = gen_init_decl_immcode(immcgen);
            break;
        case SRT_IDENT_DECL:
            codes = gen_identifier_decl_immcode(immcgen);
            break;
        case SRT_STR_DECL:
            codes = gen_string_decl_immcode(immcgen);
            break;
        case SRT_INIT:
            codes = gen_initializer_immcode(immcgen);
            break;
        case SRT_CMPD_STMT:
            immcgen->symbol_table = symboltable_enter_scope(immcgen->symbol_table);
            immcgen->tag_table = tagtable_enter_scope(immcgen->tag_table);
            codes = gen_compound_stmt_immcode(immcgen);
            immcgen->tag_table = tagtable_exit_scope(immcgen->tag_table);
            immcgen->symbol_table = symboltable_exit_scope(immcgen->symbol_table);
            break;
        case SRT_CONTINUE_STMT:
            codes = gen_continue_stmt_immcode(immcgen);
            break;
        case SRT_BREAK_STMT:
            codes = gen_break_stmt_immcode(immcgen);
            break;
        case SRT_RET_STMT:
            codes = gen_return_stmt_immcode(immcgen);
            break;
        case SRT_EXPR_STMT:
            codes = gen_expression_stmt_immcode(immcgen);
            break;
        case SRT_NULL_STMT:
            codes = gen_null_stmt_immcode();
            break;
        case SRT_IF_STMT:
            codes = gen_if_else_stmt_immcode(immcgen);
            break;
        case SRT_WHILE_STMT:
            codes = gen_while_stmt_immcode(immcgen);
            break;
        case SRT_FOR_STMT:
            immcgen->symbol_table = symboltable_enter_scope(immcgen->symbol_table);
            immcgen->tag_table = tagtable_enter_scope(immcgen->tag_table);
            codes = gen_for_stmt_immcode(immcgen);
            immcgen->tag_table = tagtable_exit_scope(immcgen->tag_table);
            immcgen->symbol_table = symboltable_exit_scope(immcgen->symbol_table);
            break;
        case SRT_ASSIGN_EXPR:
            codes = gen_assignment_expr_immcode(immcgen);
            break;
        case SRT_COND_EXPR:
            codes = gen_conditional_expr_immcode(immcgen);
            break;
        case SRT_LOR_EXPR:
            codes = gen_logical_or_expr_immcode(immcgen);
            break;
        case SRT_LAND_EXPR:
            codes = gen_logical_and_expr_immcode(immcgen);
            break;
        case SRT_OR_EXPR:
        case SRT_XOR_EXPR:
        case SRT_AND_EXPR:
            codes = gen_bitwise_expr_immcode(immcgen);
            break;
        case SRT_EQUAL_EXPR:
        case SRT_NEQUAL_EXPR:
            codes = gen_equality_expr_immcode(immcgen);
            break;
        case SRT_LESS_EXPR:
        case SRT_GREATER_EXPR:
        case SRT_LESSEQ_EXPR:
        case SRT_GREATEREQ_EXPR:
            codes = gen_relational_expr_immcode(immcgen);
            break;
        case SRT_ADD_EXPR:
        case SRT_SUB_EXPR:
            codes = gen_additive_expr_immcode(immcgen);
            break;
        case SRT_PADD_EXPR:
        case SRT_PSUB_EXPR:
            codes = gen_pointer_additive_expr_immcode(immcgen);
            break;
        case SRT_PDIFF_EXPR:
            codes = gen_pointer_diff_expr_immcode(immcgen);
            break;
        case SRT_MUL_EXPR:
        case SRT_DIV_EXPR:
        case SRT_MOD_EXPR:
            codes = gen_multiplicative_expr_immcode(immcgen);
            break;
        case SRT_PREDEC_EXPR:
        case SRT_PREINC_EXPR:
        case SRT_ADDR_EXPR:
        case SRT_INDIR_EXPR:
        case SRT_POS_EXPR:
        case SRT_NEG_EXPR:
        case SRT_NOT_EXPR:
        case SRT_LNOT_EXPR:
            codes = gen_unary_expr_immcode(immcgen);
            break;
        case SRT_CALL_EXPR:
        case SRT_TOMEMBER_EXPR:
        case SRT_POSTINC_EXPR:
        case SRT_POSTDEC_EXPR:
            codes = gen_postfix_expr_immcode(immcgen);
            break;
        case SRT_IDENT_EXPR:
        case SRT_STR_EXPR:
        case SRT_ILITERAL_EXPR:
            codes = gen_primary_expr_immcode(immcgen);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in immcgen_generate_immcode)\n");
            exit(1);
    }

    return codes;
}

void delete_immcgen(Immcgen* immcgen) {
    delete_srt(immcgen->srt);
    delete_symboltable(immcgen->symbol_table);
    delete_tagtable(immcgen->tag_table);
    if (immcgen->expr_reg != NULL) {
        delete_immcreg(immcgen->expr_reg);
    }
    if (immcgen->initialized_dtype != NULL) {
        delete_dtype(immcgen->initialized_dtype);
    }
    free(immcgen);
}
