#ifndef UMLCC_RESOLVER_STATEMENT_H
#define UMLCC_RESOLVER_STATEMENT_H

#include "./resolver.h"

ResolverReturn* resolve_stmt(Resolver* resolver);
ResolverReturn* resolve_case_stmt(Resolver* resolver);
ResolverReturn* resolve_default_stmt(Resolver* resolver);
ResolverReturn* resolve_compound_stmt(Resolver* resolver);
ResolverReturn* resolve_continue_stmt(Resolver* resolver);
ResolverReturn* resolve_break_stmt(Resolver* resolver);
ResolverReturn* resolve_return_stmt(Resolver* resolver);
ResolverReturn* resolve_expression_stmt(Resolver* resolver);
ResolverReturn* resolve_null_stmt(void);
ResolverReturn* resolve_if_else_stmt(Resolver* resolver);
ResolverReturn* resolve_switch_stmt(Resolver* resolver);
ResolverReturn* resolve_while_stmt(Resolver* resolver);
ResolverReturn* resolve_for_stmt(Resolver* resolver);

#endif
