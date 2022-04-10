#include "./parser.h"
#include "../token/token.h"
#include "../ast/ast.h"

#include <stdio.h>
#include <stdlib.h>


struct _Parser {
    Vector* tokens;
    int index;
};

Ast* parse_expr(Parser* parser);
Ast* parse_additive_expr(Parser* parser);
Ast* parse_multiplicative_expr(Parser* parser);
Ast* parse_primary_expr(Parser* parser);
void consume_token(Parser* parser, TokenType token_type);

Parser* new_parser(Vector* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->index = 0;
    return parser;
}

void delete_parser(Parser* parser) {
    delete_vector(parser->tokens, (void (*)(void* item))delete_token);
    free(parser);
}

Ast* parser_create_ast(Parser* parser) {
    Ast* ast = parse_expr(parser);
    return ast;
}

Ast* parse_expr(Parser* parser) {
    Ast* ast = parse_additive_expr(parser);
    return ast;
}

Ast* parse_additive_expr(Parser* parser) {
    Ast* ast = parse_multiplicative_expr(parser);

    while (1) {
        Token* token = vector_at(parser->tokens, parser->index);
        switch (token->type) {
            case TOKEN_PLUS:
                parser->index++;
                ast = new_ast(AST_ADD_EXPR, 2, ast, parse_multiplicative_expr(parser));
                break;
            case TOKEN_MINUS:
                parser->index++;
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
        Token* token = vector_at(parser->tokens, parser->index);
        switch (token->type) {
            case TOKEN_ASTERISK:
                parser->index++;
                ast = new_ast(AST_MUL_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            case TOKEN_SLASH:
                parser->index++;
                ast = new_ast(AST_DIV_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            case TOKEN_PERCENT:
                parser->index++;
                ast = new_ast(AST_MOD_EXPR, 2, ast, parse_primary_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;

    Token* token = vector_at(parser->tokens, parser->index);
    switch (token->type) {
        case TOKEN_INT:
            parser->index++;
            ast = new_integer_ast(token->value_int);
            break;
        case TOKEN_LPALEN:
            parser->index++;
            ast = parse_expr(parser);
            consume_token(parser, TOKEN_RPALEN);
            break;
        default:
            fprintf(stderr, "Error: unexpected token type %d\n", token->type);
            exit(1);
    }

    return ast;
}

void consume_token(Parser* parser, TokenType token_type) {
    Token* token = vector_at(parser->tokens, parser->index);
    if (token->type == token_type) {
        parser->index++;
        return;
    }
    fprintf(stderr, "Error: unexpected token type %d\n", token->type);
    exit(1);
}