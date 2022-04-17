#ifndef _UMLCC_PARSER_STATEMENT_H_
#define _UMLCC_PARSER_STATEMENT_H_

#include "./parser.h"


Ast* parse_stmt(Parser* parser);
Ast* parse_compound_stmt(Parser* parser);
Ast* parse_expression_stmt(Parser* parser);

#endif
