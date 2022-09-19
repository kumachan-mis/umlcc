#ifndef UMLCC_RESOLVER_DECLARATION_H
#define UMLCC_RESOLVER_DECLARATION_H

#include "./resolver.h"

ResolverReturn* resolve_decl(Resolver* resolver);
ResolverReturnDtype* resolve_decl_specifiers(Resolver* resolver);
ResolverReturn* resolve_init_declarator_list(Resolver* resolver);
ResolverReturn* resolve_init_declarator(Resolver* resolver);
ResolverReturn* resolve_declarator(Resolver* resolver);
ResolverReturnDParams* resolve_parameter_list(Resolver* resolver);
ResolverReturnDParam* resolve_parameter_decl(Resolver* resolver);
ResolverReturn* resolve_initializer(Resolver* resolver);

#endif
