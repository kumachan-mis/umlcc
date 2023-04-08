#include "./expression.h"
#include "../common/type.h"
#include "../ctoken/ctoken.h"
#include "./declaration.h"
#include "./util.h"

#include <stdlib.h>

ParserReturn* parse_expr(Parser* parser) {
    ParserReturn* parserret = parse_assignment_expr(parser);
    return parserret;
}

ParserReturn* parse_constant_expr(Parser* parser) {
    ParserReturn* parserret = parse_logical_or_expr(parser);
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
        if (err != NULL) {
            parser->index = index;
            delete_error(err);
            err = NULL;
            break;
        }

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_EQUAL:
                parser->index++;
                vector_push(stack, new_ast(AST_ASSIGN_EXPR, 1, ast));
                break;
            default:
                parser->index = index;
                delete_ast(ast);
                terminated = 1;
                break;
        }
    }

    parserret_assign(&ast, &err, parse_logical_or_expr(parser));
    if (err != NULL) {
        delete_vector(stack);
        return new_parserret_error(err);
    }

    vector_push(stack, ast);

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
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_logical_and_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_VBAR_VBAR:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_logical_and_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_LOR_EXPR, 2, ast, child_ast);
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
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_equality_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_AND_AND:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_equality_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_LAND_EXPR, 2, ast, child_ast);
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
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_additive_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_EQUAL_EQUAL:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_additive_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_EQUAL_EXPR, 2, ast, child_ast);
                break;
            case CTOKEN_EXCLAM_EQUAL:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_additive_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_NEQUAL_EXPR, 2, ast, child_ast);
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
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_multiplicative_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_PLUS:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_multiplicative_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_ADD_EXPR, 2, ast, child_ast);
                break;
            case CTOKEN_MINUS:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_multiplicative_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_SUB_EXPR, 2, ast, child_ast);
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
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_cast_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_ASTERISK:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_cast_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_MUL_EXPR, 2, ast, child_ast);
                break;
            case CTOKEN_SLASH:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_cast_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_DIV_EXPR, 2, ast, child_ast);
                break;
            case CTOKEN_PERCENT:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_cast_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_MOD_EXPR, 2, ast, child_ast);
                break;
            default:
                return new_parserret(ast);
        }
    }

    delete_ast(ast);
    return new_parserret_error(err);
}

ParserReturn* parse_cast_expr(Parser* parser) {
    Vector* stack = new_vector(&t_ast);
    Ast* ast = NULL;
    Ast* child_ast = NULL;
    Error* err = NULL;

    while (err == NULL) {
        int index = parser->index;
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_LPALEN) break;
        parser->index++;

        parserret_assign(&child_ast, &err, parse_type_name(parser));
        if (err != NULL) {
            parser->index = index;
            delete_error(err);
            err = NULL;
            break;
        }

        vector_push(stack, new_ast(AST_CAST_EXPR, 1, child_ast));
        err = consume_ctoken(parser, CTOKEN_RPALEN);
    }

    if (err != NULL) {
        delete_vector(stack);
        return new_parserret_error(err);
    }

    parserret_assign(&ast, &err, parse_unary_expr(parser));
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

ParserReturn* parse_unary_expr(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_AND:
            parser->index++;
            parserret_assign(&ast, &err, parse_cast_expr(parser));
            if (err != NULL) break;
            ast = new_ast(AST_ADDR_EXPR, 1, ast);
            break;
        case CTOKEN_ASTERISK:
            parser->index++;
            parserret_assign(&ast, &err, parse_cast_expr(parser));
            if (err != NULL) break;
            ast = new_ast(AST_INDIR_EXPR, 1, ast);
            break;
        case CTOKEN_EXCLAM:
            parser->index++;
            parserret_assign(&ast, &err, parse_cast_expr(parser));
            if (err != NULL) break;
            ast = new_ast(AST_LNOT_EXPR, 1, ast);
            break;
        case CTOKEN_KEYWORD_SIZEOF: {
            parser->index++;

            int index = parser->index;
            parserret_assign(&ast, &err, parse_unary_expr(parser));
            if (err == NULL) {
                ast = new_ast(AST_SIZEOF_EXPR, 1, ast);
                break;
            }
            parser->index = index;
            delete_error(err);

            err = consume_ctoken(parser, CTOKEN_LPALEN);
            if (err != NULL) break;
            parserret_assign(&ast, &err, parse_type_name(parser));
            if (err != NULL) break;
            err = consume_ctoken(parser, CTOKEN_RPALEN);
            if (err != NULL) {
                delete_ast(ast);
                break;
            }
            ast = new_ast(AST_SIZEOF_EXPR, 1, ast);
            break;
        }
        default:
            parserret_assign(&ast, &err, parse_postfix_expr(parser));
            break;
    }

    if (err != NULL) return new_parserret_error(err);
    return new_parserret(ast);
}

ParserReturn* parse_postfix_expr(Parser* parser) {
    Ast* ast = NULL;
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&ast, &err, parse_primary_expr(parser));
    if (err != NULL) return new_parserret_error(err);

    while (err == NULL) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_LBRACKET:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_expr(parser));
                if (err != NULL) break;
                ast = new_ast(AST_SUBSC_EXPR, 2, ast, child_ast);
                err = consume_ctoken(parser, CTOKEN_RBRACKET);
                break;
            case CTOKEN_LPALEN:
                parser->index++;
                parserret_assign(&child_ast, &err, parse_argument_expr_list(parser));
                if (err != NULL) break;
                ast = new_ast(AST_CALL_EXPR, 2, ast, child_ast);
                err = consume_ctoken(parser, CTOKEN_RPALEN);
                break;
            case CTOKEN_DOT:
                parser->index++;
                ctoken = vector_at(parser->ctokens, parser->index);
                if (ctoken->type != CTOKEN_IDENT) {
                    err = new_error("token identifier expected, but got %s\n", ctoken_types[ctoken->type]);
                    break;
                }
                parser->index++;
                child_ast = new_identifier_ast(AST_IDENT_EXPR, new_string(ctoken->ident_name));
                ast = new_ast(AST_MEMBER_EXPR, 2, ast, child_ast);
                break;
            case CTOKEN_ARROW:
                parser->index++;
                ctoken = vector_at(parser->ctokens, parser->index);
                if (ctoken->type != CTOKEN_IDENT) {
                    err = new_error("token identifier expected, but got %s\n", ctoken_types[ctoken->type]);
                    break;
                }
                parser->index++;
                child_ast = new_identifier_ast(AST_IDENT_EXPR, new_string(ctoken->ident_name));
                ast = new_ast(AST_TOMEMBER_EXPR, 2, ast, child_ast);
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
    Ast* child_ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_RPALEN) return new_parserret(ast);

    while (1) {
        parserret_assign(&child_ast, &err, parse_assignment_expr(parser));
        if (err != NULL) break;

        vector_push(ast->children, child_ast);
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_COMMA) break;

        parser->index++;
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
