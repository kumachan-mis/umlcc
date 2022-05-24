#ifndef UMLCC_PARSER_EXPRESSION_H
#define UMLCC_PARSER_EXPRESSION_H

#include "./parser.h"

Ast* parse_expr(Parser* parser);
Ast* parse_assignment_expr(Parser* parser);
Ast* parse_logical_or_expr(Parser* parser);
Ast* parse_logical_and_expr(Parser* parser);
Ast* parse_equality_expr(Parser* parser);
Ast* parse_additive_expr(Parser* parser);
Ast* parse_multiplicative_expr(Parser* parser);
Ast* parse_unary_expr(Parser* parser);
Ast* parse_postfix_expr(Parser* parser);
Ast* parse_argument_expr_list(Parser* parser);
Ast* parse_primary_expr(Parser* parser);

#endif
