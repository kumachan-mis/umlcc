#include "./util.h"
#include "./declaration.h"

#include <stdlib.h>

ErrorableInt* external_may_function_definition(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    int index = parser->index;
    Set* typedef_names_set = set_copy(parser->typedef_names_set);

    parserret_assign(&ast, &err, parse_decl_specifiers(parser));
    if (err != NULL) {
        delete_set(typedef_names_set);
        return new_errint_error(err);
    }
    delete_ast(ast);

    parserret_assign(&ast, &err, parse_declarator(parser));
    if (err != NULL) {
        delete_set(typedef_names_set);
        return new_errint_error(err);
    }
    delete_ast(ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);

    delete_set(parser->typedef_names_set);
    parser->index = index;
    parser->typedef_names_set = typedef_names_set;

    return new_errint(ctoken->type == CTOKEN_LBRACE);
}

ErrorableInt* blockitem_may_decl(Parser* parser) {

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    switch (ctoken->type) {
        case CTOKEN_KEYWORD_TYPEDEF:
        case CTOKEN_KEYWORD_CHAR:
        case CTOKEN_KEYWORD_INT:
            return new_errint(1);
        case CTOKEN_IDENT:
            return new_errint(set_contains(parser->typedef_names_set, ctoken->ident_name));
        default:
            return new_errint(0);
    }
}

Error* consume_ctoken(Parser* parser, CTokenType ctoken_type) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == ctoken_type) {
        parser->index++;
        return NULL;
    }
    return new_error("token %s expected, but got %s\n", ctoken_types[ctoken_type], ctoken_types[ctoken->type]);
}
