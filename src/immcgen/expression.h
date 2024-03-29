#ifndef UMLCC_IMMCGEN_EXPRESSION_H
#define UMLCC_IMMCGEN_EXPRESSION_H

#include "./immcgen.h"

Vector* gen_assignment_expr_immcode(Immcgen* immcgen);
Vector* gen_conditional_expr_immcode(Immcgen* immcgen);
Vector* gen_logical_or_expr_immcode(Immcgen* immcgen);
Vector* gen_logical_and_expr_immcode(Immcgen* immcgen);
Vector* gen_bitwise_expr_immcode(Immcgen* immcgen);
Vector* gen_equality_expr_immcode(Immcgen* immcgen);
Vector* gen_relational_expr_immcode(Immcgen* immcgen);
Vector* gen_shift_expr_immcode(Immcgen* immcgen);
Vector* gen_additive_expr_immcode(Immcgen* immcgen);
Vector* gen_pointer_additive_expr_immcode(Immcgen* immcgen);
Vector* gen_pointer_diff_expr_immcode(Immcgen* immcgen);
Vector* gen_multiplicative_expr_immcode(Immcgen* immcgen);
Vector* gen_unary_expr_immcode(Immcgen* immcgen);
Vector* gen_postfix_expr_immcode(Immcgen* immcgen);
Vector* gen_primary_expr_immcode(Immcgen* immcgen);

#endif
