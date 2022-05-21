#ifndef _UMLCC_PARSER_DECLARATION_H_
#define _UMLCC_PARSER_DECLARATION_H_

#include "./parser.h"

Ast* parse_decl(Parser* parser);
Ast* parse_decl_specifiers(Parser* parser);
Ast* parse_init_declarator_list(Parser* parser);
Ast* parse_init_declarator(Parser* parser);
Ast* parse_declarator(Parser* parser);
Ast* parse_pointer(Parser* parser);
Ast* parse_direct_declarator(Parser* parser);
Ast* parse_parameter_list(Parser* parser);
Ast* parse_parameter_decl(Parser* parser);

#endif
