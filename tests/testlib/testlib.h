#ifndef UMLCC_TESTLIB_H
#define UMLCC_TESTLIB_H

#include "../../src/ast/ast.h"
#include "../../src/vector/vector.h"

int testlib_ctokens_equals(Vector* actual, Vector* expected);
int testlib_ast_equals(Ast* actual, Ast* expected);

#endif
