#ifndef UMLCC_RESOLVER_STATEMENT_H
#define UMLCC_RESOLVER_STATEMENT_H

#include "./resolver.h"

ResolverReturn* resolve_stmt(Resolver* resolver);
ResolverReturn* resolve_compound_stmt(Resolver* resolver);
ResolverReturn* resolve_return_stmt(Resolver* resolver);
ResolverReturn* resolve_expression_stmt(Resolver* resolver);
ResolverReturn* resolve_if_else_stmt(Resolver* resolver);

#endif
