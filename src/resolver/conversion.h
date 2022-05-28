#ifndef UMLCC_RESOLVER_CONVERSION_H
#define UMLCC_RESOLVER_CONVERSION_H

#include "../srt/srt.h"

Srt* convert_to_ptr_if_array(Srt* srt);
Srt* convert_to_ptr_if_function(Srt* srt);

#endif
