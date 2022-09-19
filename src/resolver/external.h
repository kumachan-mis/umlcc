#ifndef UMLCC_RESOLVER_EXTERNAL_H
#define UMLCC_RESOLVER_EXTERNAL_H

#include "../srt/srt.h"
#include "./resolver.h"

ResolverReturn* resolve_transration_unit(Resolver* resolver);
ResolverReturn* resolve_function_definition(Resolver* resolver);

#endif
