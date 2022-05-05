#ifndef _UMLCC_RESOLVER_STATEMENT_H_
#define _UMLCC_RESOLVER_STATEMENT_H_

#include "./resolver.h"

Srt* resolve_stmt(Resolver* resolver);
Srt* resolve_compound_stmt(Resolver* resolver);
Srt* resolve_return_stmt(Resolver* resolver);
Srt* resolve_expression_stmt(Resolver* resolver);

#endif
