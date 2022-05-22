#include "./expression.h"
#include "../common/common.h"
#include "../token/token.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

Ast* parse_expr(Parser* parser) {
    Ast* ast = parse_assignment_expr(parser);
    return ast;
}

Ast* parse_assignment_expr(Parser* parser) {
    Vector* stack = new_vector(&t_ast);
    int terminated = 0;

    while (!terminated) {
        int index = parser->_index;
        Ast* ast = parse_unary_expr(parser);

        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_EQUAL:
                parser->_index++;
                vector_push(stack, new_ast(AST_ASSIGN_EXPR, 1, ast));
                break;
            default:
                delete_ast(ast);
                parser->_index = index;
                vector_push(stack, parse_logical_or_expr(parser));
                terminated = 1;
                break;
        }
    }

    Ast* ast = vector_pop(stack);
    while (vector_size(stack) > 0) {
        Ast* next_ast = vector_pop(stack);
        vector_push(next_ast->children, ast);
        ast = next_ast;
    }

    delete_vector(stack);

    return ast;
}

Ast* parse_logical_or_expr(Parser* parser) {
    Ast* ast = parse_logical_and_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_VBAR_VBAR:
                parser->_index++;
                ast = new_ast(AST_LOR_EXPR, 2, ast, parse_logical_and_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_logical_and_expr(Parser* parser) {
    Ast* ast = parse_equality_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_AND_AND:
                parser->_index++;
                ast = new_ast(AST_LAND_EXPR, 2, ast, parse_equality_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_equality_expr(Parser* parser) {
    Ast* ast = parse_additive_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_EQUAL_EQUAL:
                parser->_index++;
                ast = new_ast(AST_EQUAL_EXPR, 2, ast, parse_additive_expr(parser));
                break;
            case TOKEN_EXCLAM_EQUAL:
                parser->_index++;
                ast = new_ast(AST_NEQUAL_EXPR, 2, ast, parse_additive_expr(parser));
                break;
            default:
                return ast;
        }
    }
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
    Ast* ast = parse_unary_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_ASTERISK:
                parser->_index++;
                ast = new_ast(AST_MUL_EXPR, 2, ast, parse_unary_expr(parser));
                break;
            case TOKEN_SLASH:
                parser->_index++;
                ast = new_ast(AST_DIV_EXPR, 2, ast, parse_unary_expr(parser));
                break;
            case TOKEN_PERCENT:
                parser->_index++;
                ast = new_ast(AST_MOD_EXPR, 2, ast, parse_unary_expr(parser));
                break;
            default:
                return ast;
        }
    }
}

Ast* parse_unary_expr(Parser* parser) {
    Vector* stack = new_vector(&t_ast);
    int terminated = 0;

    while (!terminated) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_AND:
                parser->_index++;
                vector_push(stack, new_ast(AST_ADDR_EXPR, 0));
                break;
            case TOKEN_ASTERISK:
                parser->_index++;
                vector_push(stack, new_ast(AST_INDIR_EXPR, 0));
                break;
            case TOKEN_EXCLAM:
                parser->_index++;
                vector_push(stack, new_ast(AST_LNOT_EXPR, 0));
                break;
            default:
                terminated = 1;
                break;
        }
    }

    Ast* ast = parse_postfix_expr(parser);
    while (vector_size(stack) > 0) {
        Ast* next_ast = vector_pop(stack);
        vector_push(next_ast->children, ast);
        ast = next_ast;
    }

    delete_vector(stack);

    return ast;
}

Ast* parse_postfix_expr(Parser* parser) {
    Ast* ast = parse_primary_expr(parser);

    while (1) {
        Token* token = vector_at(parser->_tokens, parser->_index);
        switch (token->type) {
            case TOKEN_LPALEN:
                parser->_index++;
                ast = new_ast(AST_CALL_EXPR, 2, ast, parse_argument_expr_list(parser));
                consume_token(parser, TOKEN_RPALEN);
                break;
            default:
                return ast;
        }
    }

    return ast;
}

Ast* parse_argument_expr_list(Parser* parser) {
    Ast* ast = new_ast(AST_ARG_LIST, 0);

    Token* token = vector_at(parser->_tokens, parser->_index);
    if (token->type == TOKEN_RPALEN) return ast;

    while (1) {
        vector_push(ast->children, parse_assignment_expr(parser));
        token = vector_at(parser->_tokens, parser->_index);
        if (token->type == TOKEN_RPALEN) break;
        consume_token(parser, TOKEN_COMMA);
    }

    return ast;
}

Ast* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;

    Token* token = vector_at(parser->_tokens, parser->_index);
    switch (token->type) {
        case TOKEN_IDENT:
            parser->_index++;
            ast = new_identifier_ast(AST_IDENT_EXPR, new_string(token->ident_name));
            break;
        case TOKEN_INT:
            parser->_index++;
            ast = new_integer_ast(AST_INT_EXPR, token->value_int);
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
