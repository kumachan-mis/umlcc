#ifndef UMLCC_RESOLVER_DECLARATION_H
#define UMLCC_RESOLVER_DECLARATION_H

#include "./resolver.h"

Srt* resolve_decl(Resolver* resolver);
CType* resolve_decl_specifiers(Resolver* resolver);
Srt* resolve_init_declarator_list(Resolver* resolver);
Srt* resolve_init_declarator(Resolver* resolver);
Srt* resolve_declarator(Resolver* resolver);
Vector* resolve_parameter_list(Resolver* resolver);
CParam* resolve_parameter_decl(Resolver* resolver);

#endif
