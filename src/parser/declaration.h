#ifndef _UMLCC_PARSER_DECLARATION_H_
#define _UMLCC_PARSER_DECLARATION_H_

#include "./parser.h"


int  should_decl(Parser* parser);
Ast* parse_decl(Parser* parser);

#endif
