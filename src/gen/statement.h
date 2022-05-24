#ifndef UMLCC_GEN_STATEMENT_H
#define UMLCC_GEN_STATEMENT_H

#include "./gen.h"

Vector* gen_compound_stmt_code(Codegen* codegen);
Vector* gen_return_stmt_code(Codegen* codegen);
Vector* gen_expression_stmt_code(Codegen* codegen);

#endif
