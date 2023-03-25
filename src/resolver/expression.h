#ifndef UMLCC_RESOLVER_EXPRESSION_H
#define UMLCC_RESOLVER_EXPRESSION_H

#include "./resolver.h"

ResolverReturn* resolve_expr(Resolver* resolver);
ResolverReturn* resolve_assignment_expr(Resolver* resolver);
ResolverReturn* resolve_logical_expr(Resolver* resolver);
ResolverReturn* resolve_equality_expr(Resolver* resolver);
ResolverReturn* resolve_additive_expr(Resolver* resolver);
ResolverReturn* resolve_multiplicative_expr(Resolver* resolver);
ResolverReturn* resolve_cast_expr(Resolver* resolver);
ResolverReturn* resolve_unary_expr(Resolver* resolver);
ResolverReturn* resolve_postfix_expr(Resolver* resolver);
ResolverReturn* resolve_argument_expr_list(Resolver* resolver);
ResolverReturn* resolve_primary_expr(Resolver* resolver);

#endif
