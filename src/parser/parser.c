#include "./parser.h"
#include "./external.h"
#include "../token/token.h"

#include <stdlib.h>

Parser* new_parser(Vector* tokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->_tokens = tokens;
    parser->_index = 0;
    return parser;
}

void delete_parser(Parser* parser) {
    delete_vector(parser->_tokens, (void (*)(void* item))delete_token);
    free(parser);
}

Ast* parser_create_ast(Parser* parser) {
    return parse_translation_unit(parser);
}
