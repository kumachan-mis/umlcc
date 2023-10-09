#ifndef UMLCC_IMMCGEN_STATEMENT_H
#define UMLCC_IMMCGEN_STATEMENT_H

#include "./immcgen.h"

Vector* gen_compound_stmt_immcode(Immcgen* immcgen);
Vector* gen_return_stmt_immcode(Immcgen* immcgen);
Vector* gen_expression_stmt_immcode(Immcgen* immcgen);
Vector* gen_null_stmt_immcode(void);
Vector* gen_if_else_stmt_immcode(Immcgen* immcgen);
Vector* gen_while_stmt_immcode(Immcgen* immcgen);

#endif
