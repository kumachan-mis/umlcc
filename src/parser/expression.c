#include "./expression.h"
#include "../common/type.h"
#include "../ctoken/ctoken.h"
#include "./util.h"

#include <stdlib.h>

ParserReturn* parse_expr(Parser* parser) {
    ParserReturn* parserret = parse_assignment_expr(parser);
    return parserret;
}

ParserReturn* parse_assignment_expr(Parser* parser) {
    Vector* stack = new_vector(&t_ast);
    Ast* ast = NULL;
    Error* err = NULL;

    int terminated = 0;
    while (!terminated) {
        int index = parser->index;
        parserret_assign(&ast, &err, parse_unary_expr(parser));
        if (err != NULL) break;

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_EQUAL:
                parser->index++;
                vector_push(stack, new_ast(AST_ASSIGN_EXPR, 1, ast));
                break;
            default:
                delete_ast(ast);
                parser->index = index;
                parserret_assign(&ast, &err, parse_logical_or_expr(parser));
                terminated = 1;
                if (err != NULL) break;
                vector_push(stack, ast);
                break;
        }
    }

    if (err != NULL) {
        delete_vector(stack);
        return new_parserret_error(err);
    }

    ast = vector_pop(stack);
    while (vector_size(stack) > 0) {
        Ast* next_ast = vector_pop(stack);
        vector_push(next_ast->children, ast);
        ast = next_ast;
    }

    delete_vector(stack);
    return new_parserret(ast);
}

ParserReturn* parse_logical_or_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_logical_and_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_VBAR_VBAR:
                parser->index++;
                parserret_assign(&child, &err, parse_logical_and_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_LOR_EXPR, 2, ast, child);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_logical_and_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_equality_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_AND_AND:
                parser->index++;
                parserret_assign(&child, &err, parse_equality_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_LAND_EXPR, 2, ast, child);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_equality_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_additive_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_EQUAL_EQUAL:
                parser->index++;
                parserret_assign(&child, &err, parse_additive_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_EQUAL_EXPR, 2, ast, child);
                break;
            case CTOKEN_EXCLAM_EQUAL:
                parser->index++;
                parserret_assign(&child, &err, parse_additive_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_NEQUAL_EXPR, 2, ast, child);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_additive_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_multiplicative_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_PLUS:
                parser->index++;
                parserret_assign(&child, &err, parse_multiplicative_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_ADD_EXPR, 2, ast, child);
                break;
            case CTOKEN_MINUS:
                parser->index++;
                parserret_assign(&child, &err, parse_multiplicative_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_SUB_EXPR, 2, ast, child);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_multiplicative_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_unary_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_ASTERISK:
                parser->index++;
                parserret_assign(&child, &err, parse_unary_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_MUL_EXPR, 2, ast, child);
                break;
            case CTOKEN_SLASH:
                parser->index++;
                parserret_assign(&child, &err, parse_unary_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_DIV_EXPR, 2, ast, child);
                break;
            case CTOKEN_PERCENT:
                parser->index++;
                parserret_assign(&child, &err, parse_unary_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_MOD_EXPR, 2, ast, child);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_unary_expr(Parser* parser) {
    Vector* stack = new_vector(&t_ast);
    Ast* ast = NULL;
    Error* err = NULL;

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

    parserret_assign(&ast, &err, parse_postfix_expr(parser));
    if (err != NULL) {
        delete_vector(stack);
        return new_parserret_error(err);
    }

    while (vector_size(stack) > 0) {
        Ast* next_ast = vector_pop(stack);
        vector_push(next_ast->children, ast);
        ast = next_ast;
    }

    delete_vector(stack);
    return new_parserret(ast);
}

ParserReturn* parse_postfix_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_primary_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_LBRACKET:
                parser->index++;
                parserret_assign(&child, &err, parse_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_SUBSC_EXPR, 2, ast, child);
                err = consume_ctoken(parser, CTOKEN_RBRACKET);
                break;
            case CTOKEN_LPALEN:
                parser->index++;
                parserret_assign(&child, &err, parse_argument_expr_list(parser));
                if (err != NULL) break;
                ast = new_ast(AST_CALL_EXPR, 2, ast, child);
                err = consume_ctoken(parser, CTOKEN_RPALEN);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_argument_expr_list(Parser* parser) {
    Ast* ast = new_ast(AST_ARG_LIST, 0);
    Ast* child = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RPALEN) return new_parserret(ast);

    while (err == NULL) {
        parserret_assign(&child, &err, parse_assignment_expr(parser));
        if (err != NULL) break;

        vector_push(ast->children, child);
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RPALEN) break;

        err = consume_ctoken(parser, CTOKEN_COMMA);
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_primary_expr(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_IDENT:
            parser->index++;
            ast = new_identifier_ast(AST_IDENT_EXPR, new_string(ctoken->ident_name));
            break;
        case CTOKEN_INT:
            parser->index++;
            ast = new_iliteral_ast(AST_INT_EXPR, iliteral_copy(ctoken->iliteral));
            break;
        case CTOKEN_CHAR:
            parser->index++;
            ast = new_iliteral_ast(AST_CHAR_EXPR, iliteral_copy(ctoken->iliteral));
            break;
        case CTOKEN_STRING:
            parser->index++;
            ast = new_sliteral_ast(AST_STRING_EXPR, sliteral_copy(ctoken->sliteral));
            break;
        case CTOKEN_LPALEN:
            parser->index++;
            parserret_assign(&ast, &err, parse_expr(parser));
            if (err != NULL) break;
            err = consume_ctoken(parser, CTOKEN_RPALEN);
            break;
        default:
            err = new_error("unexpected token %s\n", ctoken_types[ctoken->type]);
            break;
    }

    if (err != NULL) {
        if (ast != NULL) delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}
