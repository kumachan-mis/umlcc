#include "./expression.h"
#include "./util.h"
#include "../token/token.h"

#include <stdio.h>
#include <stdlib.h>


Ast* parse_additive_expr(Parser* parser);
Ast* parse_multiplicative_expr(Parser* parser);
Ast* parse_primary_expr(Parser* parser);


Ast* parse_expr(Parser* parser) {
    Ast* ast = parse_additive_expr(parser);
    return ast;
}

Ast* parse_additive_expr(Parser* parser) {
    Ast* ast = parse_multiplicative_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_PLUS:
                parser->_index++;
                ast = new_ast(AST_ADD_EXPR, 2, ast, parse_multiplicative_expr(parser));
                break;
            case TOKEN_MINUS:
                parser->_index++;
                ast = new_ast(AST_SUB_EXPR, 2, ast, parse_multiplicative_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_multiplicative_expr(Parser* parser) {
    Ast* ast = parse_primary_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_ASTERISK:
                parser->_index++;
                ast = new_ast(AST_MUL_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            case TOKEN_SLASH:
                parser->_index++;
                ast = new_ast(AST_DIV_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            case TOKEN_PERCENT:
                parser->_index++;
                ast = new_ast(AST_MOD_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;

    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_IDENT: {
            char* ident_name = malloc((strlen(token->ident_name) + 1) * sizeof(char));
            strcpy(ident_name, token->ident_name);
            ast = new_identifier_ast(ident_name);
            break;
        }
        case TOKEN_INT:
            parser->_index++;
            ast = new_integer_ast(token->value_int);
            break;
        case TOKEN_LPALEN:
            parser->_index++;
            ast = parse_expr(parser);
            consume_token(parser, TOKEN_RPALEN);
            break;
        default:
            fprintf(stderr, "Error: unexpected token type %d\n", token->type);
            exit(1);
    }

    return ast;
}
