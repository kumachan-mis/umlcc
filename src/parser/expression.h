#ifndef UMLCC_PARSER_EXPRESSION_H
#define UMLCC_PARSER_EXPRESSION_H

#include "./parser.h"

ParserReturn* parse_expr(Parser* parser);
ParserReturn* parse_constant_expr(Parser* parser);
ParserReturn* parse_assignment_expr(Parser* parser);
ParserReturn* parse_conditional_expr(Parser* parser);
ParserReturn* parse_logical_or_expr(Parser* parser);
ParserReturn* parse_logical_and_expr(Parser* parser);
ParserReturn* parse_bitwise_inclusive_or_expr(Parser* parser);
ParserReturn* parse_bitwise_exclusive_or_expr(Parser* parser);
ParserReturn* parse_bitwise_and_expr(Parser* parser);
ParserReturn* parse_equality_expr(Parser* parser);
ParserReturn* parse_relational_expr(Parser* parser);
ParserReturn* parse_additive_expr(Parser* parser);
ParserReturn* parse_multiplicative_expr(Parser* parser);
ParserReturn* parse_cast_expr(Parser* parser);
ParserReturn* parse_unary_expr(Parser* parser);
ParserReturn* parse_postfix_expr(Parser* parser);
ParserReturn* parse_argument_expr_list(Parser* parser);
ParserReturn* parse_primary_expr(Parser* parser);

#endif
