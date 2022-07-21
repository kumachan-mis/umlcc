#include "./expression.h"
#include "../common/type.h"
#include "../ctoken/ctoken.h"
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
        int index = parser->index;
        Ast* ast = parse_unary_expr(parser);

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_EQUAL:
                parser->index++;
                vector_push(stack, new_ast(AST_ASSIGN_EXPR, 1, ast));
                break;
            default:
                delete_ast(ast);
                parser->index = index;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_VBAR_VBAR:
                parser->index++;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_AND_AND:
                parser->index++;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_EQUAL_EQUAL:
                parser->index++;
                ast = new_ast(AST_EQUAL_EXPR, 2, ast, parse_additive_expr(parser));
                break;
            case CTOKEN_EXCLAM_EQUAL:
                parser->index++;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_PLUS:
                parser->index++;
                ast = new_ast(AST_ADD_EXPR, 2, ast, parse_multiplicative_expr(parser));
                break;
            case CTOKEN_MINUS:
                parser->index++;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_ASTERISK:
                parser->index++;
                ast = new_ast(AST_MUL_EXPR, 2, ast, parse_unary_expr(parser));
                break;
            case CTOKEN_SLASH:
                parser->index++;
                ast = new_ast(AST_DIV_EXPR, 2, ast, parse_unary_expr(parser));
                break;
            case CTOKEN_PERCENT:
                parser->index++;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_AND:
                parser->index++;
                vector_push(stack, new_ast(AST_ADDR_EXPR, 0));
                break;
            case CTOKEN_ASTERISK:
                parser->index++;
                vector_push(stack, new_ast(AST_INDIR_EXPR, 0));
                break;
            case CTOKEN_EXCLAM:
                parser->index++;
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
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_LBRACKET:
                parser->index++;
                ast = new_ast(AST_SUBSC_EXPR, 2, ast, parse_expr(parser));
                consume_ctoken(parser, CTOKEN_RBRACKET);
                break;
            case CTOKEN_LPALEN:
                parser->index++;
                ast = new_ast(AST_CALL_EXPR, 2, ast, parse_argument_expr_list(parser));
                consume_ctoken(parser, CTOKEN_RPALEN);
                break;
            default:
                return ast;
        }
    }

    return ast;
}

Ast* parse_argument_expr_list(Parser* parser) {
    Ast* ast = new_ast(AST_ARG_LIST, 0);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RPALEN) return ast;

    while (1) {
        vector_push(ast->children, parse_assignment_expr(parser));
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RPALEN) break;
        consume_ctoken(parser, CTOKEN_COMMA);
    }

    return ast;
}

Ast* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_IDENT:
            parser->index++;
            ast = new_identifier_ast(AST_IDENT_EXPR, new_string(ctoken->ident_name));
            break;
        case CTOKEN_INT:
            parser->index++;
            ast = new_integer_ast(AST_INT_EXPR, ctoken->iliteral->signed_value);
            break;
        case CTOKEN_CHAR:
            parser->index++;
            ast = new_integer_ast(AST_CHAR_EXPR, ctoken->iliteral->signed_value);
            break;
        case CTOKEN_STRING:
            parser->index++;
            ast = new_sliteral_ast(AST_STRING_EXPR, sliteral_copy(ctoken->sliteral));
            break;
        case CTOKEN_LPALEN:
            parser->index++;
            ast = parse_expr(parser);
            consume_ctoken(parser, CTOKEN_RPALEN);
            break;
        default:
            fprintf(stderr, "Error: unexpected ctoken type %d\n", ctoken->type);
            exit(1);
    }

    return ast;
}
