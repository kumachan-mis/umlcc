#ifndef UMLCC_RESOLVER_EXPRESSION_H
#define UMLCC_RESOLVER_EXPRESSION_H

#include "./resolver.h"

Srt* resolve_expr(Resolver* resolver);
Srt* resolve_assignment_expr(Resolver* resolver);
Srt* resolve_logical_expr(Resolver* resolver);
Srt* resolve_equality_expr(Resolver* resolver);
Srt* resolve_additive_expr(Resolver* resolver);
Srt* resolve_multiplicative_expr(Resolver* resolver);
Srt* resolve_unary_expr(Resolver* resolver);
Srt* resolve_postfix_expr(Resolver* resolver);
Srt* resolve_argument_expr_list(Resolver* resolver);
Srt* resolve_primary_expr(Resolver* resolver);

#endif
