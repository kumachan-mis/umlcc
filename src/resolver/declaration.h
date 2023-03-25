#ifndef UMLCC_RESOLVER_DECLARATION_H
#define UMLCC_RESOLVER_DECLARATION_H

#include "./resolver.h"

ResolverReturn* resolve_decl(Resolver* resolver);
ResolverDTypeReturn* resolve_decl_specifiers(Resolver* resolver);
ResolverDTypeReturn* resolve_specifier_qualifier_list(Resolver* resolver);
ResolverDTypeReturn* resolve_struct_specifier(Resolver* resolver);
ResolverReturnDStructMembers* resolve_struct_decl_list(Resolver* resolver);
ResolverReturnDStructMembers* resolve_struct_decl(Resolver* resolver);
ResolverReturnDStructMembers* resolve_struct_declarator_list(Resolver* resolver);
ResolverReturnDStructMember* resolve_struct_declarator(Resolver* resolver);
ResolverDTypeReturn* resolve_enum_specifier(Resolver* resolver);
ResolverReturnDEnumMembers* resolve_enumerator_list(Resolver* resolver);
ResolverReturnDEnumMember* resolve_enumerator(Resolver* resolver);
ResolverReturn* resolve_init_declarator_list(Resolver* resolver);
ResolverReturn* resolve_init_declarator(Resolver* resolver);
ResolverReturn* resolve_declarator(Resolver* resolver);
ResolverReturnDParams* resolve_parameter_list(Resolver* resolver);
ResolverReturnDParam* resolve_parameter_decl(Resolver* resolver);
ResolverDTypeReturn* resolve_type_name(Resolver* resolver);
ResolverReturn* resolve_initializer(Resolver* resolver);

#endif
