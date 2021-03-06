#include "./util.h"
#include "./declaration.h"

#include <stdio.h>
#include <stdlib.h>

int external_may_function_definition(Parser* parser) {
    Ast* ast = NULL;
    int index = parser->index;
    Set* typedef_names_set = set_copy(parser->typedef_names_set);

    ast = parse_decl_specifiers(parser);
    delete_ast(ast);
    ast = parse_declarator(parser);
    delete_ast(ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);

    delete_set(parser->typedef_names_set);
    parser->index = index;
    parser->typedef_names_set = typedef_names_set;

    return ctoken->type == CTOKEN_LBRACE;
}

int blockitem_may_decl(Parser* parser) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_KEYWORD_TYPEDEF:
        case CTOKEN_KEYWORD_CHAR:
        case CTOKEN_KEYWORD_INT:
            return 1;
        case CTOKEN_IDENT:
            return set_contains(parser->typedef_names_set, ctoken->ident_name);
        default:
            return 0;
    }
}

void consume_ctoken(Parser* parser, CTokenType ctoken_type) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == ctoken_type) {
        parser->index++;
        return;
    }
    fprintf(stderr, "Error: unexpected ctoken type %d\n", ctoken->type);
    exit(1);
}
