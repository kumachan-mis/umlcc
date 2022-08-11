#include "./test_external.h"
#include "../../src/resolver/resolver.h"
#include "../testlib/testlib.h"

void test_resolve_transration_unit();

void run_resolver_test(Ast* __restrict__ input, Srt* __restrict__ expected);

CU_Suite* add_test_suite_external_resolver() {
    CU_Suite* suite = CU_add_suite("test_suite_external_resolver", NULL, NULL);
    CU_ADD_TEST(suite, test_resolve_transration_unit);
    return suite;
}

void test_resolve_transration_unit() {
    Ast* input = new_ast(
        AST_TRAS_UNIT, 2,
        new_ast(
            AST_DECL, 2,                    // non-terminal
            new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                    new_ast(AST_TYPE_INT, 0)),
            new_ast(AST_INIT_DECLOR_LIST, 1,            // non-terminal
                    new_ast(AST_INIT_DECLOR, 1,         // non-terminal
                            new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                                    new_identifier_ast(AST_IDENT_DECLOR, new_string("incriment")),
                                    new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                            new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                                    new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                            new_ast(AST_TYPE_INT, 0)),
                                                    new_identifier_ast(AST_IDENT_DECLOR,
                                                                       new_string("x")))))))),
        new_ast(AST_FUNC_DEF, 3,                // non-terminal
                new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                        new_ast(AST_TYPE_INT, 0)),
                new_ast(AST_FUNC_DECLOR, 2, // non-terminal
                        new_identifier_ast(AST_IDENT_DECLOR, new_string("decriment")),
                        new_ast(AST_PARAM_LIST, 1,                      // non-terminal
                                new_ast(AST_PARAM_DECL, 2,              // non-terminal
                                        new_ast(AST_DECL_SPECIFIERS, 1, // non-terminal
                                                new_ast(AST_TYPE_INT, 0)),
                                        new_identifier_ast(AST_IDENT_DECLOR, new_string("x"))))),
                new_ast(AST_CMPD_STMT, 1,                // non-terminal
                        new_ast(AST_RET_STMT, 1,         // non-terminal
                                new_ast(AST_SUB_EXPR, 2, // non-terminal
                                        new_identifier_ast(AST_IDENT_EXPR, new_string("x")),
                                        new_iliteral_ast(AST_INT_EXPR,
                                                         new_signed_iliteral(INTEGER_INT, 1)))))));

    Vector* incr_params = new_vector(&t_dparam);
    vector_push(incr_params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    Dtype* incr_dtype = new_function_dtype(incr_params, new_integer_dtype(DTYPE_INT));

    Vector* decr_params = new_vector(&t_dparam);
    vector_push(decr_params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    Dtype* decr_dtype = new_function_dtype(decr_params, new_integer_dtype(DTYPE_INT));

    Srt* expected = new_srt(
        SRT_TRAS_UNIT, 2,                 // non-terminal
        new_srt(SRT_DECL_LIST, 1,         // non-terminal
                new_srt(SRT_INIT_DECL, 1, // non-terminal
                        new_identifier_srt(SRT_DECL, incr_dtype, new_string("incriment")))),
        new_srt(SRT_FUNC_DEF, 2, // non-terminal
                new_identifier_srt(SRT_DECL, decr_dtype, new_string("decriment")),
                new_srt(SRT_CMPD_STMT, 1,        // non-terminal
                        new_srt(SRT_RET_STMT, 1, // non-terminal
                                new_dtyped_srt(
                                    SRT_SUB_EXPR, new_integer_dtype(DTYPE_INT), 2, // non-terminal
                                    new_identifier_srt(SRT_IDENT_EXPR, new_integer_dtype(DTYPE_INT),
                                                       new_string("x")),
                                    new_iliteral_srt(SRT_INT_EXPR, new_integer_dtype(DTYPE_INT),
                                                     new_signed_iliteral(INTEGER_INT, 1)))))));

    run_resolver_test(input, expected);

    delete_srt(expected);
}

void run_resolver_test(Ast* __restrict__ input, Srt* __restrict__ expected) {
    Resolver* resolver = new_resolver(input);

    Srt* actual = resolver_resolve_semantics(resolver);

    CU_ASSERT_TRUE(testlib_srt_equals(actual, expected));

    delete_srt(actual);
    delete_resolver(resolver);
}
