#ifndef UMLCC_PARSER_STATEMENT_H
#define UMLCC_PARSER_STATEMENT_H

#include "./parser.h"

ParserReturn* parse_stmt(Parser* parser);
ParserReturn* parse_compound_stmt(Parser* parser);
ParserReturn* parse_continue_stmt(Parser* parser);
ParserReturn* parse_break_stmt(Parser* parser);
ParserReturn* parse_return_stmt(Parser* parser);
ParserReturn* parse_expression_stmt(Parser* parser);
ParserReturn* parse_if_else_stmt(Parser* parser);
ParserReturn* parse_while_stmt(Parser* parser);
ParserReturn* parse_for_stmt(Parser* parser);

#endif
