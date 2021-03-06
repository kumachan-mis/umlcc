#ifndef UMLCC_RESOLVER_DECLARATION_H
#define UMLCC_RESOLVER_DECLARATION_H

#include "./resolver.h"

Srt* resolve_decl(Resolver* resolver);
Dtype* resolve_decl_specifiers(Resolver* resolver);
Srt* resolve_init_declarator_list(Resolver* resolver);
Srt* resolve_init_declarator(Resolver* resolver);
Srt* resolve_declarator(Resolver* resolver);
Vector* resolve_parameter_list(Resolver* resolver);
DParam* resolve_parameter_decl(Resolver* resolver);
Srt* resolve_initializer(Resolver* resolver);

#endif
