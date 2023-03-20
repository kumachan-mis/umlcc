#include "./test_external.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/parser/parser.h"
#include "../testlib/testlib.h"

void test_parse_transration_unit(void);

void run_parser_test(Vector* input, Ast* expected);

CU_Suite* add_test_suite_external_parser(void) {
    CU_Suite* suite = CU_add_suite("test_suite_external_parser", NULL, NULL);
    CU_ADD_TEST(suite, test_parse_transration_unit);
    return suite;
}

void test_parse_transration_unit(void) {
    Vector* input = new_vector(&t_ctoken);
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("incriment")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("decriment")));
    vector_push(input, new_ctoken(CTOKEN_LPALEN));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_INT));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_RPALEN));
    vector_push(input, new_ctoken(CTOKEN_LBRACE));
    vector_push(input, new_ctoken(CTOKEN_KEYWORD_RETURN));
    vector_push(input, new_identifier_ctoken(CTOKEN_IDENT, new_string("x")));
    vector_push(input, new_ctoken(CTOKEN_MINUS));
    vector_push(input, new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, 1)));
    vector_push(input, new_ctoken(CTOKEN_SEMICOLON));
    vector_push(input, new_ctoken(CTOKEN_RBRACE));
    vector_push(input, new_ctoken(CTOKEN_EOF));

    Ast* expected =
        new_ast(AST_TRAS_UNIT, 2,
                new_ast(AST_DECL, 2,               // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                                new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("incriment")),
                                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                                        new_ast(AST_PARAM_DECL, 1,         // non-terminal
                                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                                        new_ast(AST_TYPE_INT, 0)))))))),
                new_ast(AST_FUNC_DEF, 3,           // non-terminal
                        new_ast(AST_DECL_SPECS, 1, // non-terminal
                                new_ast(AST_TYPE_INT, 0)),
                        new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                new_identifier_ast(AST_IDENT_DECLOR, new_string("decriment")),
                                new_ast(AST_PARAM_LIST, 1,                 // non-terminal
                                        new_ast(AST_PARAM_DECL, 2,         // non-terminal
                                                new_ast(AST_DECL_SPECS, 1, // non-terminal
                                                        new_ast(AST_TYPE_INT, 0)),
                                                new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))),
                        new_ast(AST_CMPD_STMT, 1,                // non-terminal
                                new_ast(AST_RET_STMT, 1,         // non-terminal
                                        new_ast(AST_SUB_EXPR, 2, // non-terminal
                                                new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                                new_iliteral_ast(AST_INT_EXPR, new_signed_iliteral(INTEGER_INT, 1))))))

        );

    run_parser_test(input, expected);

    delete_ast(expected);
}

void run_parser_test(Vector* input, Ast* expected) {
    Parser* parser = new_parser(input);
    Ast* actual = NULL;
    Error* err = NULL;
    parserret_assign(&actual, &err, parser_create_ast(parser));

    testlib_assert_ast_equal(actual, expected);
    CU_ASSERT_PTR_NULL(err);

    if (actual != NULL) delete_ast(actual);
    delete_parser(parser);
}
