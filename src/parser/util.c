#include "./util.h"
#include "./declaration.h"

#include <stdlib.h>

ErrorableInt* external_may_function_definition(Parser* parser) {
    Ast* ast = NULL;
    Error* err = NULL;

    int index = parser->index;
    Set* original_typedef_names_set = parser->typedef_names_set;
    int original_typedef_flag = parser->typedef_flag;

    parser->typedef_names_set = set_copy(parser->typedef_names_set);
    parserret_assign(&ast, &err, parse_decl_specifiers(parser));

    if (err != NULL) {
        delete_set(parser->typedef_names_set);
        parser->index = index;
        parser->typedef_names_set = original_typedef_names_set;
        parser->typedef_flag = original_typedef_flag;
        return new_errint_error(err);
    }
    delete_ast(ast);

    parserret_assign(&ast, &err, parse_declarator(parser));

    delete_set(parser->typedef_names_set);
    parser->typedef_names_set = original_typedef_names_set;
    parser->typedef_flag = original_typedef_flag;

    if (err != NULL) {
        parser->index = index;
        delete_error(err);
        return new_errint(0);
    }
    delete_ast(ast);

    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    parser->index = index;
    return new_errint(ctoken->type == CTOKEN_LBRACE);
}

int item_may_decl(Parser* parser) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    return ctoken_is_storage_class_specifier(ctoken) || ctoken_is_type_specifier(ctoken, parser->typedef_names_set);
}

int ctoken_is_storage_class_specifier(CToken* ctoken) {
    return ctoken->type == CTOKEN_KEYWORD_TYPEDEF;
}

int ctoken_is_type_specifier(CToken* ctoken, Set* typedef_names_set) {
    return ctoken->type == CTOKEN_KEYWORD_VOID || ctoken->type == CTOKEN_KEYWORD_CHAR ||
           ctoken->type == CTOKEN_KEYWORD_INT || ctoken->type == CTOKEN_KEYWORD_LONG ||
           ctoken->type == CTOKEN_KEYWORD_UNSIGNED || ctoken->type == CTOKEN_KEYWORD_STRUCT ||
           ctoken->type == CTOKEN_KEYWORD_ENUM ||
           (ctoken->type == CTOKEN_IDENT && set_contains(typedef_names_set, ctoken->ident_name));
}

Error* consume_ctoken(Parser* parser, CTokenType ctoken_type) {
    CToken* ctoken = vector_at(parser->ctokens, parser->index);
    if (ctoken->type == ctoken_type) {
        parser->index++;
        return NULL;
    }
    return new_error("token %s expected, but got %s", ctoken_types[ctoken_type], ctoken_types[ctoken->type]);
}
