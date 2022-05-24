#include "./parser.h"
#include "../token/token.h"
#include "./external.h"

#include <stdlib.h>

Parser* new_parser(Vector* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->index = 0;
    return parser;
}

void delete_parser(Parser* parser) {
    delete_vector(parser->tokens);
    free(parser);
}

Ast* parser_create_ast(Parser* parser) {
    return parse_translation_unit(parser);
}
