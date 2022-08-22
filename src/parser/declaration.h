#ifndef UMLCC_PARSER_DECLARATION_H
#define UMLCC_PARSER_DECLARATION_H

#include "./parser.h"

ParserReturn* parse_decl(Parser* parser);
ParserReturn* parse_decl_specifiers(Parser* parser);
ParserReturn* parse_init_declarator_list(Parser* parser);
ParserReturn* parse_init_declarator(Parser* parser);
ParserReturn* parse_declarator(Parser* parser);
ParserReturn* parse_pointer(Parser* parser);
ParserReturn* parse_direct_declarator(Parser* parser);
ParserReturn* parse_parameter_list(Parser* parser);
ParserReturn* parse_parameter_decl(Parser* parser);
ParserReturn* parse_initializer(Parser* parser);
ParserReturn* parse_initializer_list(Parser* parser);

#endif
