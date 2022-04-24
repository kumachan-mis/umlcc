#ifndef _UMLCC_PARSER_EXPRESSION_H_
#define _UMLCC_PARSER_EXPRESSION_H_

#include "./parser.h"


Ast* parse_expr(Parser* parser);
Ast* parse_assignment_expr(Parser* parser);
Ast* parse_additive_expr(Parser* parser);
Ast* parse_multiplicative_expr(Parser* parser);
Ast* parse_postfix_expr(Parser* parser);
Ast* parse_primary_expr(Parser* parser);

#endif
