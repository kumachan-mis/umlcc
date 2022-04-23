#ifndef _UMLCC_RESOLVER_EXPRESSION_H_
#define _UMLCC_RESOLVER_EXPRESSION_H_

#include "./resolver.h"


Srt* resolve_expr(Resolver* resolver);
Srt* resolve_assignment_expr(Resolver* resolver);
Srt* resolve_additive_expr(Resolver* resolver);
Srt* resolve_multiplicative_expr(Resolver* resolver);
Srt* resolve_primary_expr(Resolver* resolver);

#endif
