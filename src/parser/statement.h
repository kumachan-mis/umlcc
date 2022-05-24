#ifndef UMLCC_PARSER_STATEMENT_H
#define UMLCC_PARSER_STATEMENT_H

#include "./parser.h"

Ast* parse_stmt(Parser* parser);
Ast* parse_compound_stmt(Parser* parser);
Ast* parse_return_stmt(Parser* parser);
Ast* parse_expression_stmt(Parser* parser);

#endif
