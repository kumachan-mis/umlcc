#include "./parser.h"
#include "../ctoken/ctoken.h"
#include "./external.h"

#include <stdlib.h>

Parser* new_parser(Vector* ctokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->ctokens = ctokens;
    parser->index = 0;
    return parser;
}

Ast* parser_create_ast(Parser* parser) {
    return parse_translation_unit(parser);
}

void delete_parser(Parser* parser) {
    delete_vector(parser->ctokens);
    free(parser);
}
