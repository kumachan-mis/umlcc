#ifndef UMLCC_GEN_STATEMENT_H
#define UMLCC_GEN_STATEMENT_H

#include "./immcgen.h"

Vector* gen_compound_stmt_immcode(Immcgen* immcgen);
Vector* gen_return_stmt_immcode(Immcgen* immcgen);
Vector* gen_expression_stmt_immcode(Immcgen* immcgen);

#endif
