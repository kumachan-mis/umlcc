#ifndef _UMLCC_RESOLVER_EXTERNAL_H_
#define _UMLCC_RESOLVER_EXTERNAL_H_

#include "./resolver.h"
#include "../srt/srt.h"


Srt* resolve_transration_unit(Resolver* resolver);
Srt* resolve_function_definition(Resolver* resolver);

#endif
