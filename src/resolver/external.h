#ifndef UMLCC_RESOLVER_EXTERNAL_H
#define UMLCC_RESOLVER_EXTERNAL_H

#include "../srt/srt.h"
#include "./resolver.h"

Srt* resolve_transration_unit(Resolver* resolver);
Srt* resolve_function_definition(Resolver* resolver);

#endif
