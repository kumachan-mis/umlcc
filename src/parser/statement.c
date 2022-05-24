#include "./statement.h"
#include "./declaration.h"
#include "./expression.h"
#include "./util.h"

Ast* parse_stmt(Parser* parser) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_LBRACE:
            return parse_compound_stmt(parser);
        case CTOKEN_KEYWORD_RETURN:
            return parse_return_stmt(parser);
        default:
            return parse_expression_stmt(parser);
    }
}

Ast* parse_compound_stmt(Parser* parser) {
    Ast* ast = new_ast(AST_CMPD_STMT, 0);

    consume_ctoken(parser, CTOKEN_LBRACE);
    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) {
            parser->index++;
            return ast;
        }
        if (blockitem_may_decl(parser)) {
            vector_push(ast->children, parse_decl(parser));
        } else {
            vector_push(ast->children, parse_stmt(parser));
        }
    }
}

Ast* parse_return_stmt(Parser* parser) {
    consume_ctoken(parser, CTOKEN_KEYWORD_RETURN);
    Ast* expr = parse_expr(parser);
    consume_ctoken(parser, CTOKEN_SEMICOLON);
    return new_ast(AST_RET_STMT, 1, expr);
}

Ast* parse_expression_stmt(Parser* parser) {
    Ast* expr = parse_expr(parser);
    consume_ctoken(parser, CTOKEN_SEMICOLON);
    return new_ast(AST_EXPR_STMT, 1, expr);
}
