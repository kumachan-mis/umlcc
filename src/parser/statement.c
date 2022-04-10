#include "./statement.h"
#include "./expression.h"
#include "./util.h"


Ast* parse_compound_stmt(Parser* parser);
Ast* parse_expression_stmt(Parser* parser);

Ast* parse_stmt(Parser* parser) {
    Ast* ast = parse_compound_stmt(parser);
    return ast;
}

Ast* parse_compound_stmt(Parser* parser) {
    Ast* ast = new_ast(AST_CMPD_STMT, 0);

    consume_token(parser, TOKEN_LBRACE);
    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        if (token->type == TOKEN_RBRACE) {
            parser->_index++;
            return ast;
        }
        vector_push(ast->children, parse_expression_stmt(parser));
    }
}

Ast* parse_expression_stmt(Parser* parser) {
    Ast* expr = parse_expr(parser);
    consume_token(parser, TOKEN_SEMICOLON);
    return new_ast(AST_EXPR_STMT, 1, expr);
}
