#include "./declaration.h"
#include "../common/type.h"
#include "../ctoken/ctoken.h"
#include "./expression.h"
#include "./util.h"

#include <stdio.h>
#include <stdlib.h>

ParserReturn* parse_declarator_common(Parser* parser, int abstract);
ParserReturn* parse_direct_declarator_common(Parser* parser, int abstract);

ParserReturn* parse_decl(Parser* parser) {
    Ast* ast = new_ast(AST_DECL, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&child_ast, &err, parse_decl_specifiers(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_SEMICOLON) {
        parser->index++;
        return new_parserret(ast);
    }

    parserret_assign(&child_ast, &err, parse_init_declarator_list(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    return new_parserret(ast);
}

ParserReturn* parse_type_name(Parser* parser) {
    Ast* ast = new_ast(AST_TYPE_NAME, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&child_ast, &err, parse_specifier_qualifier_list(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    int index = parser->index;
    parserret_assign(&child_ast, &err, parse_abstract_declarator(parser));
    if (err != NULL) {
        parser->index = index;
        delete_error(err);
        return new_parserret(ast);
    }

    vector_push(ast->children, child_ast);

    return new_parserret(ast);
}

ParserReturn* parse_decl_specifiers(Parser* parser) {
    Ast* ast = new_ast(AST_DECL_SPECS, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;
    CToken* ctoken = NULL;
    int typedef_flag = 0;

    while (1) {
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken_is_storage_class_specifier(ctoken)) {
            parserret_assign(&child_ast, &err, parse_storage_class_specifier(parser));
            typedef_flag = typedef_flag || child_ast->type == AST_STG_TYPEDEF;
        } else if (ctoken_is_type_specifier(ctoken, parser->typedef_names_set)) {
            parserret_assign(&child_ast, &err, parse_type_specifier(parser));
        } else {
            break;
        }

        if (err != NULL) break;
        vector_push(ast->children, child_ast);
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    if (vector_size(ast->children) == 0) {
        delete_ast(ast);
        ctoken = vector_at(parser->ctokens, parser->index);
        err = new_error("one of declaration specifiers expected, but got %s\n", ctoken_types[ctoken->type]);
        return new_parserret_error(err);
    }

    parser->typedef_flag = typedef_flag;
    return new_parserret(ast);
}

ParserReturn* parse_specifier_qualifier_list(Parser* parser) {
    Ast* ast = new_ast(AST_SPEC_QUAL_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;
    CToken* ctoken = NULL;

    // TODO: consider type qualifiers

    while (1) {
        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken_is_type_specifier(ctoken, parser->typedef_names_set)) {
            parserret_assign(&child_ast, &err, parse_type_specifier(parser));
        } else {
            break;
        }

        if (err != NULL) break;
        vector_push(ast->children, child_ast);
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    if (vector_size(ast->children) == 0) {
        delete_ast(ast);
        ctoken = vector_at(parser->ctokens, parser->index);
        err = new_error("one of type specifiers or type qualifiers expected, but got %s\n", ctoken_types[ctoken->type]);
        return new_parserret_error(err);
    }

    return new_parserret(ast);
}

ParserReturn* parse_storage_class_specifier(Parser* parser) {
    Ast* ast = NULL;
    CToken* ctoken = vector_at(parser->ctokens, parser->index);

    switch (ctoken->type) {
        case CTOKEN_KEYWORD_TYPEDEF:
            parser->index++;
            ast = new_ast(AST_STG_TYPEDEF, 0);
            break;
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in parse_storage_class_specifier)\n");
            exit(1);
    }

    return new_parserret(ast);
}

ParserReturn* parse_type_specifier(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;
    CToken* ctoken = vector_at(parser->ctokens, parser->index);

    switch (ctoken->type) {
        case CTOKEN_KEYWORD_VOID:
            parser->index++;
            ast = new_ast(AST_TYPE_VOID, 0);
            break;
        case CTOKEN_KEYWORD_CHAR:
            parser->index++;
            ast = new_ast(AST_TYPE_CHAR, 0);
            break;
        case CTOKEN_KEYWORD_INT:
            parser->index++;
            ast = new_ast(AST_TYPE_INT, 0);
            break;
        case CTOKEN_KEYWORD_STRUCT:
            parserret_assign(&ast, &err, parse_struct_specifier(parser));
            break;
        case CTOKEN_KEYWORD_ENUM:
            parserret_assign(&ast, &err, parse_enum_specifier(parser));
            break;
        case CTOKEN_IDENT:
            if (set_contains(parser->typedef_names_set, ctoken->ident_name)) {
                parser->index++;
                ast = new_identifier_ast(AST_TYPEDEF_NAME, new_string(ctoken->ident_name));
                break;
            }
            // fall through
        default:
            fprintf(stderr, "\x1b[1;31mfatal error\x1b[0m: "
                            "unreachable statement (in parse_type_specifier)\n");
            exit(1);
    }

    if (err != NULL) return new_parserret_error(err);
    return new_parserret(ast);
}

ParserReturn* parse_struct_specifier(Parser* parser) {
    Ast* ast = new_ast(AST_TYPE_STRUCT, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_STRUCT);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    CToken* ctoken = NULL;

    ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_IDENT) {
        parser->index++;
        child_ast = new_identifier_ast(AST_STRUCT_NAME, new_string(ctoken->ident_name));
        vector_push(ast->children, child_ast);
    }

    ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_LBRACE) {
        parser->index++;
        parserret_assign(&child_ast, &err, parse_struct_decl_list(parser));
        if (err != NULL) {
            delete_ast(ast);
            return new_parserret_error(err);
        }

        vector_push(ast->children, child_ast);

        err = consume_ctoken(parser, CTOKEN_RBRACE);
        if (err != NULL) {
            delete_ast(ast);
            return new_parserret_error(err);
        }
    }

    if (vector_size(ast->children) == 0) {
        err = new_error("identifier or { expected, but got %s\n", ctoken_types[ctoken->type]);
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_struct_decl_list(Parser* parser) {
    Ast* ast = new_ast(AST_STRUCT_DECL_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    while (1) {
        parserret_assign(&child_ast, &err, parse_struct_decl(parser));
        if (err != NULL) break;

        vector_push(ast->children, child_ast);
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_struct_decl(Parser* parser) {
    Ast* ast = new_ast(AST_STRUCT_DECL, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&child_ast, &err, parse_specifier_qualifier_list(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    parserret_assign(&child_ast, &err, parse_struct_declarator_list(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    err = consume_ctoken(parser, CTOKEN_SEMICOLON);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    return new_parserret(ast);
}

ParserReturn* parse_struct_declarator_list(Parser* parser) {
    Ast* ast = new_ast(AST_STRUCT_DECLOR_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    while (1) {
        parserret_assign(&child_ast, &err, parse_declarator(parser));
        if (err != NULL) break;
        vector_push(ast->children, child_ast);
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_COMMA) break;
        parser->index++;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_enum_specifier(Parser* parser) {
    Ast* ast = new_ast(AST_TYPE_ENUM, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_KEYWORD_ENUM);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    CToken* ctoken = NULL;

    ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_IDENT) {
        parser->index++;
        child_ast = new_identifier_ast(AST_ENUM_NAME, new_string(ctoken->ident_name));
        vector_push(ast->children, child_ast);
    }

    ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_LBRACE) {
        parser->index++;
        parserret_assign(&child_ast, &err, parse_enumerator_list(parser));
        if (err != NULL) {
            delete_ast(ast);
            return new_parserret_error(err);
        }

        vector_push(ast->children, child_ast);

        err = consume_ctoken(parser, CTOKEN_RBRACE);
        if (err != NULL) {
            delete_ast(ast);
            return new_parserret_error(err);
        }
    }

    if (vector_size(ast->children) == 0) {
        err = new_error("identifier or { expected, but got %s\n", ctoken_types[ctoken->type]);
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_enumerator_list(Parser* parser) {
    Ast* ast = new_ast(AST_ENUMOR_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    while (1) {
        parserret_assign(&child_ast, &err, parse_enumerator(parser));
        if (err != NULL) break;
        vector_push(ast->children, child_ast);

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;

        err = consume_ctoken(parser, CTOKEN_COMMA);
        if (err != NULL) break;

        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_enumerator(Parser* parser) {
    Ast* ast = new_ast(AST_ENUMOR, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    CToken* ctoken = ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_IDENT) {
        err = new_error("identifier expected, but got %s\n", ctoken_types[ctoken->type]);
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, new_identifier_ast(AST_ENUM_CONST, new_string(ctoken->ident_name)));
    parser->index++;

    ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_EQUAL) return new_parserret(ast);
    parser->index++;

    parserret_assign(&child_ast, &err, parse_constant_expr(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);
    return new_parserret(ast);
}

ParserReturn* parse_init_declarator_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;
    int typedef_flag = parser->typedef_flag;

    while (1) {
        parser->typedef_flag = typedef_flag;
        parserret_assign(&child_ast, &err, parse_init_declarator(parser));
        if (err != NULL) break;

        vector_push(ast->children, child_ast);
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_COMMA) break;

        parser->index++;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_init_declarator(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_DECLOR, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&child_ast, &err, parse_declarator(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_EQUAL) return new_parserret(ast);

    parser->index++;
    parserret_assign(&child_ast, &err, parse_initializer(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    vector_push(ast->children, child_ast);

    return new_parserret(ast);
}

ParserReturn* parse_declarator(Parser* parser) {
    return parse_declarator_common(parser, 0);
}

ParserReturn* parse_abstract_declarator(Parser* parser) {
    return parse_declarator_common(parser, 1);
}

ParserReturn* parse_declarator_common(Parser* parser, int abstract) {
    Ast* pointer_ast = NULL;
    Ast* direct_declarator_ast = NULL;
    Error* err = NULL;

    AstType terminal_ast_type = abstract ? AST_ABS_DECLOR : AST_IDENT_DECLOR;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == CTOKEN_ASTERISK) {
        parserret_assign(&pointer_ast, &err, parse_pointer(parser));
        if (err != NULL) return new_parserret_error(err);
    }

    if (abstract) {
        parserret_assign(&direct_declarator_ast, &err, parse_direct_abstract_declarator(parser));
    } else {
        parserret_assign(&direct_declarator_ast, &err, parse_direct_declarator(parser));
    }

    if (err != NULL) {
        if (pointer_ast != NULL) delete_ast(pointer_ast);
        return new_parserret_error(err);
    }

    if (pointer_ast == NULL) return new_parserret(direct_declarator_ast);

    Ast* pointer_tail_ast = pointer_ast;
    while (vector_size(pointer_tail_ast->children) > 0) {
        pointer_tail_ast = vector_at(pointer_tail_ast->children, 0);
    }

    if (direct_declarator_ast->type == terminal_ast_type) {
        vector_push(pointer_tail_ast->children, direct_declarator_ast);
        return new_parserret(pointer_ast);
    }

    Ast* direct_declarator_tail_ast = direct_declarator_ast;
    while (1) {
        Ast* child_ast = vector_at(direct_declarator_tail_ast->children, 0);
        if (child_ast->type == terminal_ast_type) break;
        direct_declarator_tail_ast = child_ast;
    }

    Ast* terminal_ast = ast_copy(vector_at(direct_declarator_tail_ast->children, 0));
    vector_set(direct_declarator_tail_ast->children, 0, pointer_ast);
    vector_push(pointer_tail_ast->children, terminal_ast);
    return new_parserret(direct_declarator_ast);
}

ParserReturn* parse_pointer(Parser* parser) {
    Ast* ast = new_ast(AST_PTR_DECLOR, 0);
    Error* err = NULL;

    err = consume_ctoken(parser, CTOKEN_ASTERISK);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    while (1) {
        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_ASTERISK) break;
        parser->index++;
        ast = new_ast(AST_PTR_DECLOR, 1, ast);
    }

    return new_parserret(ast);
}

ParserReturn* parse_direct_declarator(Parser* parser) {
    return parse_direct_declarator_common(parser, 0);
}

ParserReturn* parse_direct_abstract_declarator(Parser* parser) {
    return parse_direct_declarator_common(parser, 1);
}

ParserReturn* parse_direct_declarator_common(Parser* parser, int abstract) {
    Ast* ast = NULL;
    Error* err = NULL;

    Ast* head_ast = NULL;
    Ast* tail_ast = NULL;
    Ast* enclosed_head_ast = NULL;
    Ast* enclosed_tail_ast = NULL;
    Ast* terminal_ast = NULL;

    AstType terminal_ast_type = abstract ? AST_ABS_DECLOR : AST_IDENT_DECLOR;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_LPALEN:
            parser->index++;
            if (abstract) {
                parserret_assign(&enclosed_head_ast, &err, parse_abstract_declarator(parser));
            } else {
                parserret_assign(&enclosed_head_ast, &err, parse_declarator(parser));
            }
            if (err != NULL) return new_parserret_error(err);
            err = consume_ctoken(parser, CTOKEN_RPALEN);
            if (err != NULL) return new_parserret_error(err);

            if (enclosed_head_ast->type == terminal_ast_type) {
                terminal_ast = enclosed_head_ast;
                enclosed_head_ast = NULL;
                break;
            }

            enclosed_tail_ast = enclosed_head_ast;
            while (1) {
                Ast* child_ast = vector_at(enclosed_tail_ast->children, 0);
                if (child_ast->type == terminal_ast_type) {
                    terminal_ast = ast_copy(child_ast);
                    vector_erase(enclosed_tail_ast->children, 0);
                    break;
                }
                enclosed_tail_ast = child_ast;
            }
            break;
        case CTOKEN_IDENT:
            if (abstract) {
                err = new_error("unexpected token indentifier\n");
                return new_parserret_error(err);
            }

            parser->index++;
            terminal_ast = new_identifier_ast(AST_IDENT_DECLOR, new_string(ctoken->ident_name));
            if (parser->typedef_flag) {
                set_add(parser->typedef_names_set, new_string(terminal_ast->ident_name));
                parser->typedef_flag = 0;
            }
            break;
        default:
            if (abstract) {
                terminal_ast = new_ast(AST_ABS_DECLOR, 0);
                break;
            }

            err = new_error("unexpected token %s\n", ctoken_types[ctoken->type]);
            return new_parserret_error(err);
    }

    while (err == NULL) {
        ctoken = vector_at(parser->ctokens, parser->index);
        switch (ctoken->type) {
            case CTOKEN_LBRACKET: {
                Ast* child_ast = NULL;
                parser->index++;
                parserret_assign(&child_ast, &err, parse_constant_expr(parser));
                if (err != NULL) break;

                Ast* declarator_ast = new_ast(AST_ARRAY_DECLOR, 1, child_ast);
                if (head_ast == NULL) head_ast = declarator_ast;
                if (tail_ast != NULL) vector_insert(tail_ast->children, 0, declarator_ast);
                tail_ast = declarator_ast;
                err = consume_ctoken(parser, CTOKEN_RBRACKET);
                break;
            }
            case CTOKEN_LPALEN: {
                Ast* child_ast = NULL;
                parser->index++;
                parserret_assign(&child_ast, &err, parse_parameter_list(parser));
                if (err != NULL) break;

                Ast* declarator_ast = new_ast(AST_FUNC_DECLOR, 1, child_ast);
                if (head_ast == NULL) head_ast = declarator_ast;
                if (tail_ast != NULL) vector_insert(tail_ast->children, 0, declarator_ast);
                tail_ast = declarator_ast;
                err = consume_ctoken(parser, CTOKEN_RPALEN);
                break;
            }
            default:
                if (enclosed_head_ast != NULL && head_ast != NULL) {
                    ast = enclosed_head_ast;
                    vector_insert(enclosed_tail_ast->children, 0, head_ast);
                    vector_insert(tail_ast->children, 0, terminal_ast);
                } else if (enclosed_head_ast != NULL) {
                    ast = enclosed_head_ast;
                    vector_insert(enclosed_tail_ast->children, 0, terminal_ast);
                } else if (head_ast != NULL) {
                    ast = head_ast;
                    vector_insert(tail_ast->children, 0, terminal_ast);
                } else {
                    ast = terminal_ast;
                }
                return new_parserret(ast);
        }
    }

    delete_ast(terminal_ast);
    if (head_ast != NULL) delete_ast(head_ast);
    if (enclosed_head_ast != NULL) delete_ast(enclosed_head_ast);
    return new_parserret_error(err);
}

ParserReturn* parse_parameter_list(Parser* parser) {
    Ast* ast = new_ast(AST_PARAM_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;
    int typedef_flag = parser->typedef_flag;

    while (1) {
        parser->typedef_flag = typedef_flag;
        parserret_assign(&child_ast, &err, parse_parameter_decl(parser));
        if (err != NULL) break;
        vector_push(ast->children, child_ast);

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type != CTOKEN_COMMA) break;

        parser->index++;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_parameter_decl(Parser* parser) {
    Ast* ast = new_ast(AST_PARAM_DECL, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    parserret_assign(&child_ast, &err, parse_decl_specifiers(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);

    int index = index = parser->index;
    parserret_assign(&child_ast, &err, parse_declarator(parser));
    if (err == NULL) {
        vector_push(ast->children, child_ast);
        return new_parserret(ast);
    }

    delete_error(err);

    parser->index = index;
    parserret_assign(&child_ast, &err, parse_abstract_declarator(parser));
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }

    vector_push(ast->children, child_ast);
    return new_parserret(ast);
}

ParserReturn* parse_initializer(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type != CTOKEN_LBRACE) {
        parserret_assign(&ast, &err, parse_assignment_expr(parser));
        if (err != NULL) return new_parserret_error(err);
        return new_parserret(ast);
    }

    parser->index++;
    parserret_assign(&ast, &err, parse_initializer_list(parser));
    if (err != NULL) return new_parserret_error(err);

    err = consume_ctoken(parser, CTOKEN_RBRACE);
    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}

ParserReturn* parse_initializer_list(Parser* parser) {
    Ast* ast = new_ast(AST_INIT_LIST, 0);
    Ast* child_ast = NULL;
    Error* err = NULL;

    while (1) {
        parserret_assign(&child_ast, &err, parse_initializer(parser));
        if (err != NULL) break;
        vector_push(ast->children, child_ast);

        CToken* ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;

        err = consume_ctoken(parser, CTOKEN_COMMA);
        if (err != NULL) break;

        ctoken = vector_at(parser->ctokens, parser->index);
        if (ctoken->type == CTOKEN_RBRACE) break;
    }

    if (err != NULL) {
        delete_ast(ast);
        return new_parserret_error(err);
    }
    return new_parserret(ast);
}
