#ifndef UMLCC_RESOLVER_UTIL_H
#define UMLCC_RESOLVER_UTIL_H

#include "../ast/ast.h"

void sort_ast_type(AstType* ast_types, int begin, int end);
int ast_is_void_parameter_list(Ast* ast);

#endif
