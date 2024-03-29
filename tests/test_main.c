#include "./ast/test_ast.h"
#include "./ctoken/test_ctoken.h"
#include "./dtype/test_dtype.h"
#include "./error/test_error.h"
#include "./freader/test_freader.h"
#include "./freader/test_freader_error.h"
#include "./immc/test_immc.h"
#include "./immc/test_ope.h"
#include "./immc/test_suffix.h"
#include "./immcgen/test_declaration.h"
#include "./immcgen/test_expression.h"
#include "./immcgen/test_external.h"
#include "./immcgen/test_statement.h"
#include "./lexer/test_lexer.h"
#include "./lexer/test_lexer_error.h"
#include "./literal/test_iliteral.h"
#include "./literal/test_sliteral.h"
#include "./liveseq/test_liveness.h"
#include "./liveseq/test_liveseq.h"
#include "./map/test_map.h"
#include "./pair/test_pair.h"
#include "./parser/test_declaration.h"
#include "./parser/test_declaration_error.h"
#include "./parser/test_expression.h"
#include "./parser/test_expression_error.h"
#include "./parser/test_external.h"
#include "./parser/test_external_error.h"
#include "./parser/test_statement.h"
#include "./parser/test_statement_error.h"
#include "./regalloc/test_basicblock.h"
#include "./regalloc/test_regalloc.h"
#include "./resolver/test_conversion.h"
#include "./resolver/test_declaration.h"
#include "./resolver/test_declaration_error.h"
#include "./resolver/test_expression.h"
#include "./resolver/test_expression_error.h"
#include "./resolver/test_external.h"
#include "./resolver/test_external_error.h"
#include "./resolver/test_statement.h"
#include "./resolver/test_statement_error.h"
#include "./set/test_set.h"
#include "./srt/test_srt.h"
#include "./symtab/test_symbol.h"
#include "./symtab/test_symtab.h"
#include "./tagtab/test_tagtab.h"
#include "./vector/test_vector.h"
#include "./x64/test_ope.h"
#include "./x64/test_suffix.h"
#include "./x64/test_x64.h"
#include "./x64gen/test_arithinst.h"
#include "./x64gen/test_bitinst.h"
#include "./x64gen/test_ctrlinst.h"
#include "./x64gen/test_data.h"
#include "./x64gen/test_datainst.h"
#include "./x64gen/test_label.h"
#include "./x64gen/test_logicinst.h"
#include "./x64gen/test_shrotinst.h"
#include "./x64gen/test_x64gen.h"

#include <CUnit/Basic.h>

int main(void) {
    CU_initialize_registry();

    // container tests
    add_test_suite_pair();
    add_test_suite_vector();
    add_test_suite_map();
    add_test_suite_set();

    // cross-sectional data structure tests
    add_test_suite_iliteral();
    add_test_suite_sliteral();
    add_test_suite_dtype();
    add_test_suite_symbol();
    add_test_suite_symboltable();
    add_test_suite_tagtable();
    add_test_suite_liveness();
    add_test_suite_liveseq();
    add_test_suite_error();

    // internal data structure tests
    add_test_suite_basicblock();

    // intermediate expression tests
    add_test_suite_ctoken();
    add_test_suite_ast();
    add_test_suite_srt();
    add_test_suite_immcsuffix();
    add_test_suite_immcope();
    add_test_suite_immc();
    add_test_suite_x64suffix();
    add_test_suite_x64ope();
    add_test_suite_x64();

    // freader tests
    add_test_suite_freader();
    add_test_suite_freader_error();

    // lexer tests
    add_test_suite_lexer();
    add_test_suite_lexer_error();

    // parser tests
    add_test_suite_expr_parser();
    add_test_suite_expr_parser_error();
    add_test_suite_stmt_parser();
    add_test_suite_stmt_parser_error();
    add_test_suite_decl_parser();
    add_test_suite_decl_parser_error();
    add_test_suite_external_parser();
    add_test_suite_external_parser_error();

    // resolver tests
    add_test_suite_expr_resolver();
    add_test_suite_expr_resolver_error();
    add_test_suite_conv_resolver();
    add_test_suite_stmt_resolver();
    add_test_suite_stmt_resolver_error();
    add_test_suite_decl_resolver();
    add_test_suite_decl_resolver_error();
    add_test_suite_external_resolver();
    add_test_suite_external_resolver_error();

    // immcgen tests
    add_test_suite_expr_immcgen();
    add_test_suite_stmt_immcgen();
    add_test_suite_decl_immcgen();
    add_test_suite_exteral_immcgen();

    // regalloc tests
    add_test_suite_regalloc();

    // x64gen tests
    add_test_suite_datainst_x64gen();
    add_test_suite_arithinst_x64gen();
    add_test_suite_logicinst_x64gen();
    add_test_suite_shrotinst_x64gen();
    add_test_suite_bitinst_x64gen();
    add_test_suite_ctrlinst_x64gen();
    add_test_suite_data_x64gen();
    add_test_suite_label_x64gen();
    add_test_suite_x64gen();

    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();

    int exit_code = CU_get_number_of_failures();
    CU_cleanup_registry();
    return exit_code;
}
