#ifndef UMLCC_TESTLIB_H
#define UMLCC_TESTLIB_H

#include "../../src/ast/ast.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/immc/immc.h"
#include "../../src/literal/iliteral.h"
#include "../../src/literal/sliteral.h"
#include "../../src/liveseq/liveness.h"
#include "../../src/liveseq/liveseq.h"
#include "../../src/srt/srt.h"
#include "../../src/vector/vector.h"
#include "../../src/x64/x64.h"

int testlib_ctokens_equals(Vector* actual, Vector* expected);
int testlib_ctoken_equals(CToken* actual, CToken* expected);
int testlib_ast_equals(Ast* actual, Ast* expected);
int testlib_srt_equals(Srt* actual, Srt* expected);
int testlib_immcs_equals(Vector* actual, Vector* expected);
int testlib_immc_equals(Immc* actual, Immc* expected);
int testlib_immcope_equals(ImmcOpe* actual, ImmcOpe* expected);
int testlib_x64codes_equals(Vector* actual, Vector* expected);
int testlib_x64code_equals(X64* actual, X64* expected);
int testlib_x64ope_equals(X64Ope* actual, X64Ope* expected);
int testlib_dtype_equals(Dtype* actual, Dtype* expected);
int testlib_liveseqs_equals(Vector* actual, Vector* expected);
int testlib_liveseq_equals(Liveseq* actual, Liveseq* expected);
int testlib_liveness_equals(Liveness* actual, Liveness* expected);
int testlib_iliteral_equals(IntegerLiteral* actual, IntegerLiteral* expected);
int testlib_sliteral_equals(StringLiteral* actual, StringLiteral* expected);
int testlib_string_equals(char* actual, char* expected);

#endif
