#ifndef _UMLCC_RESOLVER_DECLARATION_H_
#define _UMLCC_RESOLVER_DECLARATION_H_

#include "./resolver.h"


Vector* resolve_decl(Resolver* resolver);
CType*  resolve_decl_specifiers(Resolver* resolver);
Vector* resolve_init_declarator_list(Resolver* resolver);
Srt*    resolve_init_declarator(Resolver* resolver);
Srt*    resolve_declarator(Resolver* resolver);
Srt*    resolve_direct_declarator(Resolver* resolver);

#endif
