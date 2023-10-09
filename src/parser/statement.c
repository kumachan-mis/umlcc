#include "./statement.h"
#include "./declaration.h"
#include "./expression.h"
#include "./util.h"

#include <stdlib.h>

ParserReturn* parse_stmt(Parser* parser) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_LBRACE:
            return parse_compound_stmt(parser);
        case CTOKEN_KEYWORD_CONTINUE:
            return parse_continue_stmt(parser);
        case CTOKEN_KEYWORD_BREAK:
            return parse_break_stmt(parser);
        case CTOKEN_KEYWORD_RETURN:
            return parse_return_stmt(parser);
        case CTOKEN_KEYWORD_IF:
            return parse_if_else_stmt(parser);
        case CTOKEN_KEYWORD_WHILE:
            return parse_while_stmt(parser);
        case CTOKEN_KEYWORD_FOR:
            return parse_for_stmt(parser);
        default:
            return parse_expression_stmt(parser);
    }
}

ParserReturn* parse_compound_stmt(Parser* parser) {
    Ast* ast = new_ast(AST_CMPD_STMT, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_LBRACE);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    Set* typedef_names_set = set_copy(parser->typedef_names_set);

    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) {
            parser->index++;
            break;
        }

        if (item_may_decl(parser)) {
            parserret_assign(&child_ast, &err, parse_decl(parser));
        } else {
            parserret_assign(&child_ast, &err, parse_stmt(parser));
        }

        if (err != NULL) {
            break;
        }
        vector_push(ast->children, child_ast);
    }

    delete_set(parser->typedef_names_set);
    parser->typedef_names_set = typedef_names_set;

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_continue_stmt(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_CONTINUE);
    if (err != NULL) {
        return new_parserret_error(err);
    }

    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        return new_parserret_error(err);
    }

    ast = new_ast(AST_CONTINUE_STMT, 0);
    return new_parserret(ast);
}

ParserReturn* parse_break_stmt(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_BREAK);
    if (err != NULL) {
        return new_parserret_error(err);
    }

    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        return new_parserret_error(err);
    }

    ast = new_ast(AST_BREAK_STMT, 0);
    return new_parserret(ast);
}

ParserReturn* parse_return_stmt(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_RETURN);
    if (err != NULL) {
        return new_parserret_error(err);
    }
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_SEMICOLON) {
        parser->index++;
        ast = new_ast(AST_RET_STMT, 0);
        return new_parserret(ast);
    }

    parserret_assign(&ast, &err, parse_expr(parser));
    if (err != NULL) {
        return new_parserret_error(err);
    }

    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    ast = new_ast(AST_RET_STMT, 1, ast);
    return new_parserret(ast);
}

ParserReturn* parse_expression_stmt(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_SEMICOLON) {
        parser->index++;
        ast = new_ast(AST_NULL_STMT, 0);
        return new_parserret(ast);
    }

    parserret_assign(&ast, &err, parse_expr(parser));
    if (err != NULL) {
        return new_parserret_error(err);
    }
    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    ast = new_ast(AST_EXPR_STMT, 1, ast);
    return new_parserret(ast);
}

ParserReturn* parse_if_else_stmt(Parser* parser) {
    Ast* ast = new_ast(AST_IF_STMT, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_IF);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    err = consume_ctoken(parser, CTOKEN_LPAREN);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    parserret_assign(&child_ast, &err, parse_expr(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    err = consume_ctoken(parser, CTOKEN_RPAREN);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    parserret_assign(&child_ast, &err, parse_stmt(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_KEYWORD_ELSE) {
        return new_parserret(ast);
    }

    parser->index++;
    parserret_assign(&child_ast, &err, parse_stmt(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    return new_parserret(ast);
}

ParserReturn* parse_while_stmt(Parser* parser) {
    Ast* ast = new_ast(AST_WHILE_STMT, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_WHILE);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    err = consume_ctoken(parser, CTOKEN_LPAREN);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    parserret_assign(&child_ast, &err, parse_expr(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    err = consume_ctoken(parser, CTOKEN_RPAREN);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    parserret_assign(&child_ast, &err, parse_stmt(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    return new_parserret(ast);
}

ParserReturn* parse_for_stmt(Parser* parser) {
    Ast* ast = new_ast(AST_FOR_STMT, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_FOR);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    err = consume_ctoken(parser, CTOKEN_LPAREN);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    if (item_may_decl(parser)) {
        parserret_assign(&child_ast, &err, parse_decl(parser));
    } else {
        parserret_assign(&child_ast, &err, parse_expression_stmt(parser));
    }
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    parserret_assign(&child_ast, &err, parse_expression_stmt(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RPAREN) {
        parser->index++;
        vector_push(ast->children, new_ast(AST_NULL_STMT, 0));
    } else {
        parserret_assign(&child_ast, &err, parse_expr(parser));
        if (err != NULL) {
            delete_ast(ast);
            return new_parserret_error(err);
        }
        vector_push(ast->children, new_ast(AST_EXPR_STMT, 1, child_ast));

        err = consume_ctoken(parser, CTOKEN_RPAREN);
        if (err != NULL) {
            delete_ast(ast);
            return new_parserret_error(err);
        }
    }

    parserret_assign(&child_ast, &err, parse_stmt(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    return new_parserret(ast);
}
