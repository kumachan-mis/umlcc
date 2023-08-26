#ifndef UMLCC_TESTLIB_H
#define UMLCC_TESTLIB_H

#include "../../src/ast/ast.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/error/error.h"
#include "../../src/immc/immc.h"
#include "../../src/literal/iliteral.h"
#include "../../src/literal/sliteral.h"
#include "../../src/liveseq/liveness.h"
#include "../../src/liveseq/liveseq.h"
#include "../../src/srt/srt.h"
#include "../../src/vector/vector.h"
#include "../../src/x64/x64.h"

void testlib_assert_ctokens_equal(Vector* actual, Vector* expected);
void testlib_assert_ctoken_equal(CToken* actual, CToken* expected);
void testlib_assert_ast_equal(Ast* actual, Ast* expected);
void testlib_assert_srt_equal(Srt* actual, Srt* expected);
void testlib_assert_immcs_equal(Vector* actual, Vector* expected);
void testlib_assert_immc_equal(Immc* actual, Immc* expected);
void testlib_assert_immcope_equal(ImmcOpe* actual, ImmcOpe* expected);
void testlib_assert_immcreg_equal(ImmcRegister* actual, ImmcRegister* expected);
void testlib_assert_x64codes_equal(Vector* actual, Vector* expected);
void testlib_assert_x64code_equal(X64* actual, X64* expected);
void testlib_assert_x64ope_equal(X64Ope* actual, X64Ope* expected);
void testlib_assert_dtype_equal(DType* actual, DType* expected);
void testlib_assert_liveseqs_equal(Vector* actual, Vector* expected);
void testlib_assert_liveseq_equal(Liveseq* actual, Liveseq* expected);
void testlib_assert_liveness_equal(Liveness* actual, Liveness* expected);
void testlib_assert_iliteral_equal(IntegerLiteral* actual, IntegerLiteral* expected);
void testlib_assert_sliteral_equal(StringLiteral* actual, StringLiteral* expected);
void testlib_assert_errors_equal(Vector* actual, Vector* expected);
void testlib_assert_error_equal(Error* actual, Error* expected);
void testlib_assert_string_equal(char* actual, char* expected);

#endif
