#ifndef _UMLCC_RESOLVER_EXTERNAL_H_
#define _UMLCC_RESOLVER_EXTERNAL_H_

#include "../srt/srt.h"
#include "./resolver.h"

Srt* resolve_transration_unit(Resolver* resolver);
Srt* resolve_function_definition(Resolver* resolver);

#endif
