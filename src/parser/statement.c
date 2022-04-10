#include "./statement.h"
#include "./expression.h"
#include "./util.h"


Ast* parse_expression_stmt(Parser* parser);

Ast* parse_stmt(Parser* parser) {
    Ast* ast = parse_expression_stmt(parser);
    return ast;
}

Ast* parse_expression_stmt(Parser* parser) {
    Ast* expr = parse_expr(parser);
    consume_token(parser, TOKEN_SEMICOLON);
    return new_ast(AST_EXPR_STMT, 1, expr);
}
