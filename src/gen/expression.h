#ifndef _UMLCC_GEN_EXPRESSION_H_
#define _UMLCC_GEN_EXPRESSION_H_

#include "./gen.h"

Vector* gen_assignment_expr_code(Codegen* codegen);
Vector* gen_logical_expr_code(Codegen* codegen);
Vector* gen_additive_expr_code(Codegen* codegen);
Vector* gen_multiplicative_expr_code(Codegen* codegen);
Vector* gen_unary_expr_code(Codegen* codegen);
Vector* gen_postfix_expr_code(Codegen* codegen);
Vector* gen_primary_expr_code(Codegen* codegen);

#endif
