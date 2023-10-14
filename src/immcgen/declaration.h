#ifndef UMLCC_IMMCGEN_DECLARATION_H
#define UMLCC_IMMCGEN_DECLARATION_H

#include "./immcgen.h"

Vector* gen_decl_list_immcode(Immcgen* immcgen);
Vector* gen_tag_decl_immcode(Immcgen* immcgen);
Vector* gen_init_decl_immcode(Immcgen* immcgen);
Vector* gen_identifier_decl_immcode(Immcgen* immcgen);
Vector* gen_string_decl_immcode(Immcgen* immcgen);
Vector* gen_initializer_immcode(Immcgen* immcgen);

#endif
