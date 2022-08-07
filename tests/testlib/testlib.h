#ifndef UMLCC_TESTLIB_H
#define UMLCC_TESTLIB_H

#include "../../src/ast/ast.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/immc/immc.h"
#include "../../src/literal/iliteral.h"
#include "../../src/literal/sliteral.h"
#include "../../src/srt/srt.h"
#include "../../src/vector/vector.h"

int testlib_ctokens_equals(Vector* actual, Vector* expected);
int testlib_ctoken_equals(CToken* actual, CToken* expected);
int testlib_ast_equals(Ast* actual, Ast* expected);
int testlib_srt_equals(Srt* actual, Srt* expected);
int testlib_immcs_equals(Vector* actual, Vector* expected);
int testlib_immc_equals(Immc* actual, Immc* expected);
int testlib_immcope_equals(ImmcOpe* actual, ImmcOpe* expected);
int testlib_dtype_equals(Dtype* actual, Dtype* expected);
int testlib_iliteral_equals(IntegerLiteral* actual, IntegerLiteral* expected);
int testlib_sliteral_equals(StringLiteral* actual, StringLiteral* expected);
int testlib_string_equals(char* actual, char* expected);

#endif
