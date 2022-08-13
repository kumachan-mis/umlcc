#include "./ast/test_ast.h"
#include "./ctoken/test_ctoken.h"
#include "./dtype/test_dtype.h"
#include "./immc/test_immc.h"
#include "./immc/test_ope.h"
#include "./immc/test_suffix.h"
#include "./immcgen/test_declaration.h"
#include "./immcgen/test_expression.h"
#include "./immcgen/test_external.h"
#include "./immcgen/test_statement.h"
#include "./lexer/test_lexer.h"
#include "./literal/test_iliteral.h"
#include "./literal/test_sliteral.h"
#include "./liveseq/test_liveness.h"
#include "./liveseq/test_liveseq.h"
#include "./map/test_map.h"
#include "./parser/test_declaration.h"
#include "./parser/test_expression.h"
#include "./parser/test_external.h"
#include "./parser/test_statement.h"
#include "./regalloc/test_basicblock.h"
#include "./regalloc/test_regalloc.h"
#include "./resolver/test_declaration.h"
#include "./resolver/test_expression.h"
#include "./resolver/test_external.h"
#include "./resolver/test_statement.h"
#include "./set/test_set.h"
#include "./srt/test_srt.h"
#include "./symtab/test_symbol.h"
#include "./symtab/test_symtab.h"
#include "./vector/test_vector.h"
#include "./x64/test_ope.h"
#include "./x64/test_suffix.h"
#include "./x64/test_x64.h"

#include <CUnit/Basic.h>

int main(void) {
    CU_initialize_registry();

    // container tests
    add_test_suite_vector();
    add_test_suite_map();
    add_test_suite_set();

    // cross-sectional data structure tests
    add_test_suite_iliteral();
    add_test_suite_sliteral();
    add_test_suite_dtype();
    add_test_suite_symbol();
    add_test_suite_symboltable();
    add_test_suite_liveness();
    add_test_suite_liveseq();

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

    // compiler step tests
    add_test_suite_lexer();
    add_test_suite_expr_parser();
    add_test_suite_stmt_parser();
    add_test_suite_decl_parser();
    add_test_suite_external_parser();
    add_test_suite_expr_resolver();
    add_test_suite_stmt_resolver();
    add_test_suite_decl_resolver();
    add_test_suite_external_resolver();
    add_test_suite_expr_immcgen();
    add_test_suite_stmt_immcgen();
    add_test_suite_decl_immcgen();
    add_test_suite_exteral_immcgen();
    add_test_suite_regalloc();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    int exit_code = CU_get_number_of_failures();
    CU_cleanup_registry();
    return exit_code;
}
