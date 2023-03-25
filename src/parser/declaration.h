#ifndef UMLCC_PARSER_DECLARATION_H
#define UMLCC_PARSER_DECLARATION_H

#include "./parser.h"

ParserReturn* parse_decl(Parser* parser);
ParserReturn* parse_decl_specifiers(Parser* parser);
ParserReturn* parse_specifier_qualifier_list(Parser* parser);
ParserReturn* parse_storage_class_specifier(Parser* parser);
ParserReturn* parse_type_specifier(Parser* parser);
ParserReturn* parse_struct_specifier(Parser* parser);
ParserReturn* parse_struct_decl_list(Parser* parser);
ParserReturn* parse_struct_decl(Parser* parser);
ParserReturn* parse_struct_declarator_list(Parser* parser);
ParserReturn* parse_enum_specifier(Parser* parser);
ParserReturn* parse_enumerator_list(Parser* parser);
ParserReturn* parse_enumerator(Parser* parser);
ParserReturn* parse_init_declarator_list(Parser* parser);
ParserReturn* parse_init_declarator(Parser* parser);
ParserReturn* parse_declarator(Parser* parser);
ParserReturn* parse_pointer(Parser* parser);
ParserReturn* parse_direct_declarator(Parser* parser);
ParserReturn* parse_parameter_list(Parser* parser);
ParserReturn* parse_parameter_decl(Parser* parser);
ParserReturn* parse_type_name(Parser* parser);
ParserReturn* parse_abstract_declarator(Parser* parser);
ParserReturn* parse_direct_abstract_declarator(Parser* parser);
ParserReturn* parse_initializer(Parser* parser);
ParserReturn* parse_initializer_list(Parser* parser);

#endif
