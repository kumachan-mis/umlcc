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

    Set* typedef_names_set = set_copy(parser->typedef_names_set);

    consume_ctoken(parser, CTOKEN_LBRACE);
    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) {
            parser->index++;
            break;
        }
        if (blockitem_may_decl(parser)) {
            vector_push(ast->children, parse_decl(parser));
        } else {
            vector_push(ast->children, parse_stmt(parser));
        }
    }

    delete_set(parser->typedef_names_set);
    parser->typedef_names_set = typedef_names_set;

    return ast;
}

Ast* parse_return_stmt(Parser* parser) {
    consume_ctoken(parser, CTOKEN_KEYWORD_RETURN);
    Ast* expr_ast = parse_expr(parser);
    consume_ctoken(parser, CTOKEN_SEMICOLON);
    return new_ast(AST_RET_STMT, 1, expr_ast);
}

Ast* parse_expression_stmt(Parser* parser) {
    Ast* expr_ast = parse_expr(parser);
    consume_ctoken(parser, CTOKEN_SEMICOLON);
    return new_ast(AST_EXPR_STMT, 1, expr_ast);
}
