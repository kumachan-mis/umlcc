#ifndef UMLCC_RESOLVER_STATEMENT_H
#define UMLCC_RESOLVER_STATEMENT_H

#include "./resolver.h"

Srt* resolve_stmt(Resolver* resolver);
Srt* resolve_compound_stmt(Resolver* resolver);
Srt* resolve_return_stmt(Resolver* resolver);
Srt* resolve_expression_stmt(Resolver* resolver);

#endif
